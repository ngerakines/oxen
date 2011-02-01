
#include "Core.hpp"

#include "TorrentIndex.hpp"
#include "Config.hpp"

#include <iostream>

#include "libtorrent/session.hpp"

#include <boost/thread.hpp>

#include "MemoryStorage.hpp"


using namespace std;

namespace oxen {

Core::Core(Config *config, TorrentIndex *ti) : config_(config), ti_(ti), running_(true), thread_(boost::thread(boost::bind(&Core::run, this))) {
}

Core::~Core() {
	running_ = false;
	cout << "Core shutting down" << endl;
	thread_.join();
}

void Core::run() {
	cout << "Core starting up, creating session: OX 0.1.0.0" << endl;
	session_ = new libtorrent::session(libtorrent::fingerprint("OX", 0, 1, 0, 0));

	// TODO: Set max upload rate
	// session_->set_upload_rate_limit(512*1024);

	// TODO: Set max download rate
	// session_->set_download_rate_limit(512*1024);

	// TODO: Determine a random port range to listen to
	session_->listen_on(std::make_pair(61000, 61500));

	int index = 0;
	boost::filesystem::path savePath("/tmp/");

	while (running_) {

		if (ti_->size() > index) {
			for (int i = index; i < ti_->size(); i++) {
				libtorrent::torrent_info *t = ti_->at(i);
#if 1
				libtorrent::add_torrent_params parms;
				parms.save_path = savePath;
				parms.ti = t;
				parms.auto_managed = true;
				parms.storage = libtorrent::temp_storage_constructor;
				libtorrent::torrent_handle handle = session_->add_torrent(parms);
				calculatePiecePriority(t, handle);
#endif
			}
			index = ti_->size();
		}

		boost::this_thread::sleep(boost::posix_time::seconds(10));
	}
}

libtorrent::session* Core::session() {
	return session_;
}

void Core::calculatePiecePriority(libtorrent::torrent_info *torrent, libtorrent::torrent_handle handle) {
	/* Given the total size of the torrent and number of pieces, determine
	 * which pieces should be marked as priority 0 (don't download) and
	 * which should be marked 4 (normal). This should take into account
	 * the availability of a piece, time that the piece has been seeded
	 * and some sort of fuzzy ratio approximation. */
	cout << "torrent has " << torrent->num_pieces() << " pieces." << endl;
	cout << "each piece is " << torrent->piece_length() << " long." << endl;
	cout << "total size is " << torrent->total_size() << " long." << endl;

	int maxMem = config_->maxMemory() * 1048576;
	cout << "given that maxMemory is " << config_->maxMemory() << " megs (" << maxMem << ") ..." << endl;
	if (maxMem > torrent->total_size()) {
		cout << "we can seed all pieces at once." << endl;
	} else {
		cout << "we can seed " << (maxMem / torrent->piece_length()) << " pieces at once." << endl; 
		int pieceCount = (maxMem / torrent->piece_length());
		for (int i = 0; i < pieceCount; i++) {
			handle.piece_priority(i, 7);
		}
		for (int i = pieceCount; i < torrent->num_pieces(); i++) {
			handle.piece_priority(i, 0);
		}
	}
}


}

