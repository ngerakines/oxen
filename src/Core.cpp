
#include "Core.hpp"

#include "TorrentIndex.hpp"
#include "Config.hpp"

#include <iostream>

#include "libtorrent/session.hpp"

#include <boost/thread.hpp>

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
				libtorrent::add_torrent_params parms;
				parms.save_path = savePath;
				parms.ti = t;
				parms.auto_managed = true;
				session_->add_torrent(parms);
			}
			index = ti_->size();
		}

		boost::this_thread::sleep(boost::posix_time::seconds(10));
	}
}

libtorrent::session* Core::session() {
	return session_;
}

}

