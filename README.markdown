# About

# USAGE

    $ ./oxen --directory /tmp/torrents/
    DirectoryWatcher checking for new torrents in /tmp/
    Adding torrent d18fa3595643fb7bd13b25a60f4e6a13da77fcc5(1388 pieces at 524288 KiB)

# GOALS

 * Monitor a directory for torrent files
 * Periodically write out stats to a log
 * Have a "queue" of torrents which represent queues of torrent chunks.
 * Be able to delete and make unavailable chunks as their ratio is met or the seed-time window closes.
 * Be able to configure start points within a torrent by default. aka, start at the end of a torrent, middle, front, etc

# License

Copyright (c) 2011 Nick Gerakines

Open sourced under the MIT license.

