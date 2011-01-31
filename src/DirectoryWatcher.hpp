#ifndef __OXEN_DIRECTORYWATCHER_H__
#define __OXEN_DIRECTORYWATCHER_H__

#include <boost/thread.hpp>

#include "TorrentIndex.hpp"
#include "Config.hpp"

namespace oxen {

class DirectoryWatcher {
	public:
		DirectoryWatcher(Config *config, TorrentIndex *ti);
		~DirectoryWatcher();

	private:
		Config *config_;
		TorrentIndex *ti_;

		volatile bool running_;
		boost::thread thread_;

		void run();
		void check();
};

}

#endif /* __OXEN_DIRECTORYWATCHER_H__ */

