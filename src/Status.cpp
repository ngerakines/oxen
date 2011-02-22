
#include "Status.hpp"

#include <iostream>
#include <typeinfo>
#include <sstream>

#include <boost/thread.hpp>
#include "libtorrent/session.hpp"

#include "Core.hpp"
#include "Config.hpp"
#include "Utils.h"

using namespace std;

namespace oxen {

Status::Status(Config *config, Core *core) : config_(config), core_(core), running_(true), thread_(boost::thread(boost::bind(&Status::run, this))) {
}

Status::~Status() {
	running_ = false;
	LOG_INFO("Status shutting down" << endl);
	thread_.join();
}

/* This thread should also check for the availability of the seeded pieces and
 * the time in which pieces have been seeded. Once a piece has been seeded long
 * enough and it's availability is good, it should initiate an update sequence
 * that moves the piece window forward, removing pieces and disconnecting all
 * connected peers. */
void Status::run() {
	LOG_INFO("Status thread starting." << endl);
	boost::this_thread::sleep(boost::posix_time::seconds(5));

	while (running_) {
		// TODO: Consider using a scoped lock here
		// TODO: Check if core_ is null
		libtorrent::session* s = core_->session();
		// TODO: Check if session is valid and sstatus is not null
		libtorrent::session_status sstatus = s->status();
		// TODO: Create an stringstream and std / syslog it instead of cout
		stringstream ss;
		ss << "Download Rate " << round(sstatus.download_rate/1024, 2) << " kb/s (" << sstatus.total_download << ", " << sstatus.total_download/1048576 << " mb)";
		ss << " | Upload Rate " << round(sstatus.upload_rate/1024, 2) <<  " kb/s (" << sstatus.total_upload << ", " << sstatus.total_upload/1048576 << " mb)";
		if (sstatus.total_download > 0) {
			double ratio = (double) sstatus.total_upload / (double) sstatus.total_download;
			cout.precision(4);
			ss << " | Ratio " << ratio;
		}
		ss << " | " << sstatus.num_peers << " peer";
		if (sstatus.num_peers != 1) {
			ss << "s";
		}
		LOG_INFO(ss.str() << endl);
		SLEEP(10);
	}
}

// TODO: Move to a utility class?
int Status::round(int value, int places) {
	float factor = pow(10.0, places);
	int tmp = (int)(value * factor + 0.5f);
	return tmp / factor;
}

}

