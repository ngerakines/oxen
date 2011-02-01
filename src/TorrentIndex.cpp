
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

using namespace std;
using namespace libtorrent;
using namespace boost::filesystem;

namespace oxen {

TorrentIndex::TorrentIndex() {
}

TorrentIndex::~TorrentIndex() {
}

void TorrentIndex::addTorrent(boost::filesystem::path file) {
	string fileName = file.string();

	int size = file_size(fileName.c_str());
	if (size > 10 * 1000000) {
		std::cerr << "file too big (" << size << "), aborting\n";
		boost::filesystem::remove(file);
		return;
	}
	std::vector<char> buf(size);
	std::ifstream(fileName.c_str(), std::ios_base::binary).read(&buf[0], size);
	lazy_entry e;
	int ret = lazy_bdecode(&buf[0], &buf[0] + buf.size(), e);
	if (ret != 0) {
		std::cerr << "Invalid bencoding in torrent " << fileName << std::endl;
		boost::filesystem::remove(file);
		return;
	}

	torrent_info *t = new torrent_info(e);

	for (int i = 0; i < (int) torrents_.size(); i++) {
		torrent_info *currentTorrent = torrents_[i];
		if (currentTorrent->info_hash() == t->info_hash()) {
			std::cerr << "Ignoring known torrent " << fileName << " (" << t->info_hash() << ")" << endl;
			boost::filesystem::remove(file);
			return;
		}
	}

	cout << "Adding torrent " << t->info_hash() << "(" << t->num_pieces() << " pieces at " << t->piece_length() << " KiB)" << endl;

	torrents_.push_back(t);
	boost::filesystem::remove(file);
}

}

