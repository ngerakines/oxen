
#include "DirectoryWatcher.hpp"

#include "TorrentIndex.hpp"
#include "Config.hpp"

#include <boost/thread.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <iostream>

#include "Utils.h"

using namespace std;

namespace oxen {

DirectoryWatcher::DirectoryWatcher(Config *config, TorrentIndex *ti) : config_(config), ti_(ti), running_(true), thread_(boost::thread(boost::bind(&DirectoryWatcher::run, this))) {
	LOG_INFO("DirectoryWatcher checking for new torrents in " << config->directory() << endl);
}

DirectoryWatcher::~DirectoryWatcher() {
	running_ = false;
	LOG_INFO("DirectoryWatcher shutting down" << endl);
	thread_.join();
}

void DirectoryWatcher::run() {
	while (running_) {
		check();
		SLEEP(10);
	}
}

void DirectoryWatcher::check() {
	std::vector<std::string> tmpList;
	boost::filesystem::directory_iterator end_iter;
	for (boost::filesystem::directory_iterator dir_itr(config_->directory()); dir_itr != end_iter; ++dir_itr) {
		if (boost::filesystem::is_regular_file(dir_itr->status())) {
			boost::filesystem::path file = dir_itr->path();
			if (file.extension().compare(".torrent") == 0) {
				ti_->addTorrent(file);
			}
		}
	}
}

}

