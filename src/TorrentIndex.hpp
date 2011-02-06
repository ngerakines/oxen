#ifndef __OXEN_TORRENT_INDEX_H__
#define __OXEN_TORRENT_INDEX_H__

#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>
#include <boost/thread/mutex.hpp>
#include "libtorrent/torrent_info.hpp"

#include "Torrent.hpp"

namespace oxen {

class TorrentIndex {
	public:
		TorrentIndex();
		~TorrentIndex();

		void addTorrent(boost::filesystem::path file);

		bool empty() {
			boost::mutex::scoped_lock lock(mutex_);
			return torrents_.empty();
		}

		int size() {
			boost::mutex::scoped_lock lock(mutex_);
			return (int) torrents_.size();
		}

		libtorrent::torrent_info* next() {
			boost::mutex::scoped_lock lock(mutex_);
			libtorrent::torrent_info *n = torrents_.back();
			torrents_.pop_back();
			return n;
		}

	private:
		std::vector<libtorrent::torrent_info *> torrents_;
		mutable boost::mutex mutex_;
};

}

#endif /* __OXEN_TORRENT_INDEX_H__ */

