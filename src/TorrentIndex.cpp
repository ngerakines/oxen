
#include "TorrentIndex.hpp"
#include "Torrent.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <iomanip>

#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/lazy_entry.hpp"
#include "libtorrent/torrent_info.hpp"
#include <boost/filesystem/operations.hpp>
#include <boost/thread/mutex.hpp>

#include "Utils.h"

using namespace std;
using namespace libtorrent;
using namespace boost::filesystem;

namespace oxen {

TorrentIndex::TorrentIndex() { }

TorrentIndex::~TorrentIndex() { }

void TorrentIndex::addTorrent(boost::filesystem::path file) {
	boost::mutex::scoped_lock lock(mutex_);
	string fileName = file.string();

	int size = file_size(fileName.c_str());
	if (size > 10 * 1000000) {
		LOG_ERROR("file too big (" << size << "), aborting" << endl);
		boost::filesystem::remove(file);
		return;
	}
	std::vector<char> buf(size);
	std::ifstream(fileName.c_str(), std::ios_base::binary).read(&buf[0], size);
	lazy_entry e;
	int ret = lazy_bdecode(&buf[0], &buf[0] + buf.size(), e);
	if (ret != 0) {
		LOG_ERROR("Invalid bencoding in torrent " << fileName << endl);
		boost::filesystem::remove(file);
		return;
	}

	torrent_info *t = new torrent_info(e);

	for (int i = 0; i < (int) torrents_.size(); i++) {
		torrent_info *currentTorrent = torrents_[i];
		if (currentTorrent->info_hash() == t->info_hash()) {
			LOG_ERROR("Ignoring known torrent " << fileName << " (" << t->info_hash() << ")" << endl);
			boost::filesystem::remove(file);
			return;
		}
	}

	// TODO: use real log facility (ie syslog) instead of std out.
	LOG_INFO("Adding torrent " << t->info_hash() << " (" << t->num_pieces() << " pieces at " << t->piece_length() << " KiB)" << endl);

	torrents_.insert(torrents_.begin(), t);
	boost::filesystem::remove(file);
}

}

