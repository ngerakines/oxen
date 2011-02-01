include_directories(.)

find_library(BOOST_THR NAMES boost_thread boost_thread-mt)
find_library(BOOST_SYS NAMES boost_system boost_system-mt)
find_library(BOOST_FS NAMES boost_filesystem boost_filesystem-mt)
find_library(BOOST_PO NAMES boost_program_options boost_program_options-mt)
find_library(TORRENT NAMES torrent-rasterbar)

add_definitions(-Wall -Wall -Wextra -g -gstabs+)

add_executable(oxen oxen.cpp Config.cpp DirectoryWatcher.cpp TorrentIndex.cpp Torrent.cpp Core.cpp Status.cpp)
TARGET_LINK_LIBRARIES(oxen ${BOOST_SYS} ${BOOST_FS} ${BOOST_THR} ${BOOST_PO} ${TORRENT})

