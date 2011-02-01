#ifndef __OXEN_STATUS_H__
#define __OXEN_STATUS_H__

#include <boost/thread.hpp>

#include "Core.hpp"
#include "Config.hpp"

namespace oxen {

class Status {
	public:
		Status(Config *config, Core *core);
		~Status();
	private:
		Config *config_;
		Core *core_;

		volatile bool running_;
		boost::thread thread_;

		void run();
		int round(int value, int places);
};

}

#endif /* __OXEN_STATUS_H__ */

