
#include "Torrent.hpp"

namespace oxen {

Torrent::Torrent(std::string infoHash, libtorrent::torrent_info *torrentInfo) : infoHash_(infoHash), torrentInfo_(torrentInfo) {
}

Torrent::~Torrent() {
}

}
