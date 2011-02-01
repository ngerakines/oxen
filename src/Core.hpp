#ifndef __OXEN_CORE_H__
#define __OXEN_CORE_H__

#include <boost/thread.hpp>

#include "libtorrent/session.hpp"

#include "Config.hpp"
#include "TorrentIndex.hpp"

namespace oxen {

class Core {
	public:
		Core(Config *config, TorrentIndex *ti);
		~Core();

		libtorrent::session* session();

	private:
		Config *config_;
		TorrentIndex *ti_;

		volatile bool running_;
		boost::thread thread_;

		void run();

		libtorrent::session *session_;
};

}

#endif /* __OXEN_CORE_H__ */

