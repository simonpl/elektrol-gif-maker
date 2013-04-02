Elektrol-gif-maker README
=========================

Copyright
---------

Elektrol-gif-maker generates GIF-images produced by the russian satellite Elektro-l
Copyright (C) 2013 Simon Plasger

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

See COPYING for details.

Building
--------

At this time, to build elektrol-gif-maker run the following command from the current directory:

`gcc -o elektrol-gif-maker elektrol-gif-maker.c -lcurl`

The availableness of libcurl is required. For generating the GIF-Images you also need convert from ImageMagick installed. The path where it's located also has to be in your PATH. A configure script and a Makefile will be provided later.

Usage
-----

`./elektrol-gif-maker --server/-s <server> --user/-u <username> --password/-p <password> --delay/-d <delay> --outputdir/-o <outputdir> [--channels/-c <channels>] [--year/-y <year>] [--month/-m <month>] [--day <day>]`

`--server/-s <server>` defines the server where the data is located.

`--user/-u <username>` defines the username that is used to log in via FTP.

`--password/-p <password>` defines the password that is used to log in via FTP.

`--delay/-d <delay>` defines the delay that is used between the images in the GIF-Animation.

`--outputdir/-o <outputdir>` defines where the data will be saved.

`--channels/-c <channels>` defines animations for which channels should be produced. Possible Channels are 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 and RGB. If it's not defined, all Channels will be animated.

`--year/-y <year>`, `--month/-m <month>` and `--day <day>` the date whose images should be used. If not all three are defined, the date of yesterday will be used.

Example:

`./elektrol-gif-maker --server ftp.ntsomz.ru --user electro --password electro --delay 0.1 --outputdir . --channels 1 2 5 RGB --year 2012 --month 5 --day 23`
