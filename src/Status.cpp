
#include "Status.hpp"

#include <iostream>
#include <typeinfo>

#include <boost/thread.hpp>
#include "libtorrent/session.hpp"

#include "Core.hpp"
#include "Config.hpp"

using namespace std;

namespace oxen {

Status::Status(Config *config, Core *core) : config_(config), core_(core), running_(true), thread_(boost::thread(boost::bind(&Status::run, this))) {
}

Status::~Status() {
	running_ = false;
	cout << "Status shutting down" << endl;
	thread_.join();
}

/* This thread should also check for the availability of the seeded pieces and
 * the time in which pieces have been seeded. Once a piece has been seeded long
 * enough and it's availability is good, it should initiate an update sequence
 * that moves the piece window forward, removing pieces and disconnecting all
 * connected peers. */
void Status::run() {
	cout << "Status thread starting." << endl;
	boost::this_thread::sleep(boost::posix_time::seconds(5));

	libtorrent::session* s = core_->session();

	while (running_) {
		libtorrent::session_status sstatus = s->status();
		cout << "Download Rate " << round(sstatus.download_rate/1024, 2) << " kb/s (" << sstatus.total_download << ", " << sstatus.total_download/1048576 << " mb)";
		cout << " | Upload Rate " << round(sstatus.upload_rate/1024, 2) <<  " kb/s (" << sstatus.total_upload << ", " << sstatus.total_upload/1048576 << " mb)";
		if (sstatus.total_download > 0) {
			double ratio = (double) sstatus.total_upload / (double) sstatus.total_download;
			cout.precision(4);
			cout << " | Ratio " << ratio;
		}
		cout << " | " << sstatus.num_peers << " peer";
		if (sstatus.num_peers != 1) {
			cout << "s";
		}
		cout << endl;
		boost::this_thread::sleep(boost::posix_time::seconds(10));
	}
}

int Status::round(int value, int places) {
	float factor = pow(10.0, places);
	int tmp = (int)(value * factor + 0.5f);
	return tmp / factor;
}

}

