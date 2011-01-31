#ifndef __OXEN_TORRENT_INDEX_H__
#define __OXEN_TORRENT_INDEX_H__

#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>
#include "libtorrent/torrent_info.hpp"

#include "Torrent.hpp"

namespace oxen {

class TorrentIndex {
	public:
		TorrentIndex();
		~TorrentIndex();

		void addTorrent(Torrent *torrent);
		void addTorrent(boost::filesystem::path file);

	private:
		std::vector<libtorrent::torrent_info *> torrents_;
};

}

#endif /* __OXEN_TORRENT_INDEX_H__ */

