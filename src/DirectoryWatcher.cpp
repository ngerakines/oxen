
#include "DirectoryWatcher.hpp"

#include "TorrentIndex.hpp"
#include "Config.hpp"

#include <boost/thread.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <iostream>
using namespace std;

namespace oxen {

DirectoryWatcher::DirectoryWatcher(Config *config, TorrentIndex *ti) : config_(config), ti_(ti), running_(true), thread_(boost::thread(boost::bind(&DirectoryWatcher::run, this))) {
	cout << "DirectoryWatcher checking for new torrents in " << config->directory() << endl;
}

DirectoryWatcher::~DirectoryWatcher() {
	running_ = false;
	thread_.join();
}

void DirectoryWatcher::run() {
	while (running_) {
		check();
		boost::this_thread::sleep(boost::posix_time::seconds(10));
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

