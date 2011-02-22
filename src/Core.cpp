
#include "Core.hpp"

#include "TorrentIndex.hpp"
#include "Config.hpp"

#include <iostream>
#include <sstream>

#include "libtorrent/session.hpp"

#include <boost/thread.hpp>

#include "MemoryStorage.hpp"

#include "Utils.h"


using namespace std;

namespace oxen {

Core::Core(Config *config, TorrentIndex *ti) : config_(config), ti_(ti), running_(true), thread_(boost::thread(boost::bind(&Core::run, this))) {
}

Core::~Core() {
	running_ = false;
	LOG_INFO("Core shutting down." << endl);
	thread_.join();
}

void Core::run() {
	LOG_INFO("Core starting up." << endl);
	session_ = new libtorrent::session(libtorrent::fingerprint("OX", 0, 1, 0, 0));

	// TODO: Set max upload rate
	// session_->set_upload_rate_limit(512*1024);

	// TODO: Set max download rate
	// session_->set_download_rate_limit(512*1024);

	// TODO: Determine a random port range to listen to
	// session_->listen_on(std::make_pair(61000, 61500));
	session_->start_upnp();

	libtorrent::torrent_info *currentTorrent = NULL;

	while (running_) {
		if (currentTorrent != NULL) {
			bool keepGoing = true;
			if (config_->getMode() == 0) {
				libtorrent::session_status sstatus = session_->status();
				if (sstatus.total_download > 0) {
					double ratio = (double) sstatus.total_upload / (double) sstatus.total_download;
					cout.precision(4);
					if (ratio > config_->getRatio()) {
						keepGoing = false;
					}
				}
			} else {
				// TODO: Implement time based checking.
			}
			if (!keepGoing) {
				libtorrent::torrent_handle handle = session_->find_torrent(currentTorrent->info_hash());
				session_->remove_torrent(handle);
				currentTorrent = NULL;
			}
		}
		if (!currentTorrent && !ti_->empty()) {
			currentTorrent = ti_->next();
			libtorrent::add_torrent_params parms;
			parms.ti = currentTorrent;
			parms.auto_managed = true;
			parms.storage = libtorrent::temp_storage_constructor;
			libtorrent::torrent_handle handle = session_->add_torrent(parms);
			calculatePiecePriority(currentTorrent, handle);
		}

		SLEEP(30);
	}
	session_->stop_upnp();
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
	stringstream ss;
	int maxMem = config_->maxMemory() * 1048576;
	if (maxMem > torrent->total_size()) {
		ss << "The entire torrent can be seeded.";
	} else {
		int pieceCount = (maxMem / torrent->piece_length());
		ss << pieceCount << " (" << torrent->piece_length() << ") of " << torrent->num_pieces() << " (" << torrent->total_size() << ") can be seeded.";
		for (int i = 0; i < pieceCount; i++) {
			handle.piece_priority(i, 7);
		}
		for (int i = pieceCount; i < torrent->num_pieces(); i++) {
			handle.piece_priority(i, 0);
		}
	}
	LOG_INFO(ss.str() << endl);
}


}

