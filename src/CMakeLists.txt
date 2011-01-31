include_directories(. ../include/)

find_library(BOOST_THR NAMES boost_thread boost_thread-mt)
find_library(BOOST_SER NAMES boost_serialization boost_serialization-mt)
find_library(BOOST_SYS NAMES boost_system boost_system-mt)
find_library(BOOST_FS NAMES boost_filesystem boost_filesystem-mt)
find_library(BOOST_PO NAMES boost_program_options boost_program_options-mt)

add_definitions(-Wall -Wall -Wextra -g -gstabs+)

add_executable(oxen oxen.cpp)
TARGET_LINK_LIBRARIES(oxen ${BOOST_SER} ${BOOST_SYS} ${BOOST_FS} ${BOOST_THR} ${BOOST_PO})

