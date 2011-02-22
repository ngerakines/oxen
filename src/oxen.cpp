/*
** Copyright (c) 2011 Nick Gerakines
** 
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
** THE SOFTWARE.
*/

#include <signal.h>

#include <iostream>
#include <fstream>
#include <string>

#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/tokenizer.hpp>

#include "DirectoryWatcher.hpp"
#include "Config.hpp"
#include "TorrentIndex.hpp"
#include "Core.hpp"
#include "Status.hpp"

#include "Utils.h"

using namespace std;
namespace po = boost::program_options;

using namespace oxen;

volatile bool running;

void sighandler(int signum) {
	std::cout << "Received signal " << signum << std::endl;
	if (signum == SIGINT) {
		running = false;
	}
}

int main(int argc, char **argv, char ** /* **ppenv */) {
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("directory", po::value<string>(), "The directory to check for new torrents in")
		("ratio", po::value<double>(), "Target ratio")
		("hours", po::value<int>(), "Time window")
		("memory", po::value<int>(), "Memory in megs")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	Config config;

	if (vm.count("directory")) {
		config.directory(vm["directory"].as<string>());
	}
	if (vm.count("memory")) {
		config.setMaxMemory(vm["memory"].as<int>());
	}
	if (vm.count("ratio")) {
		config.setRatio(vm["ratio"].as<double>());
	}
	if (vm.count("hours")) {
		config.setMode(1);
		config.setHours(vm["hours"].as<int>());
	}

	running = true;
	signal(SIGINT, sighandler);

	// Create the torrent index manager and queue
	TorrentIndex ti;

	// Start the core session thread
	Core core(&config, &ti);

	// Start the status and monitor thread
	Status status(&config, &core);

	// Start the directory watcher to feed torrents in
	DirectoryWatcher dw(&config, &ti);


	while (running) {
		SLEEP(5);
	}

	return 0;
}

