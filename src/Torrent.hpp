#ifndef __OXEN_TORRENT_H__
#define __OXEN_TORRENT_H__

#include <string>

#include "libtorrent/torrent_info.hpp"

namespace oxen {

class Torrent {
	public:
		Torrent(std::string infoHash, libtorrent::torrent_info *torrentInfo);
		~Torrent();

		std::string infoHash() { return infoHash_; }

	private:
		std::string infoHash_;
		libtorrent::torrent_info *torrentInfo_;
};

}

#endif /* __OXEN_TORRENT_H__ */

