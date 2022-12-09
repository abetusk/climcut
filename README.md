# climcut

Command line program (CLI) for [mcut](https://github.com/cutdigital/mcut).

Also included is a bare-bones (potentially clunky) `JavaScript` port via `emscripten`.

## Usage

```
    climcut [-h] [-v] [-s subject] [-c clip] [-t op] [-o output]

  [-s subj]    subject file
  [-c clip]    clip file
  [-t op]      operation (0 - a-b, 1 - b-a, 2 - union, 3 - intersection)
  [-o ofn]     output filename (base filename in the case of outputting all connected components/groups)
  [-O fmt]     output format (stl, off, obj, gp)
  [-g group#]  group number (<0 for all, default 0)
  [-v]         version
  [-h]         help (this screen)
```

## Gotchas

`runmeifyoudare.sh` tries to install (locally) and build the appropriate dependencies.

You'll need to install `eigen3` if you haven't already.
On an Ubuntu system, this can be done via:

```
sudo apt-get install libeigen3-dev
```

`runmeifyoudare.sh` will try to compile the native command line tool but you can try running it yourself
by running going into the `source/` directory and running `./build.sh` there.
If successful, this should create a file in `bin/climcut`.
To run, you'll need to make sure the shared library path is set in your environment variable if you haven't installed
`mcut` globally.

This can be done via:

```
LD_LIBRARY_PATH=`pwd`/mcut/build/bin ./bin/climcut
```

Or:

```
export LD_LIBRARY_PATH=`pwd`/mcut/build/bin
./bin/climcut
```

For some persistence.

## Discussion

Boy is this hacky!

I wanted to make sure this work wasn't lost (and may be helpful to someone else) but I haven't followed
any good practice on making it a good project or portable, so it might be a horrible mess if you want to use it.

Some points of interest:

* modified files in the `patch/` directory exist to get rid of messages insisting you need to buy a license
  (you don't if you're using it as a FOSS project and respect the license as the `mcut` library is GPLv3 and
  this project respects that license) and to get rid of debugging messages (you can presumably use a
  `-DPROFILING_BUILD` when `cmake`int/`make`ing it)
* multiple output file formats for ease of exporting (`obj`, `off`, `gnuplot` (as `gp`), `stl` not implemented)
* ease of use with the "operations"
* project is mostly a nice wrapper around the `CSGBoolean` example provided in the original `mcut` library
* attempts to export it as a `JavaScript` library for use in the browser (my experience with `emscripten` is limited,
  so there may be better practices on exposing the underlying `mcut` functionality, user beware)

Also note that by the nature of how `mcut` works, output vertex positions might be different from input vertices to
put them in general position.
I don't know what kind of difference to expect but `mcut` puts points in "general" position (to avoid pathological
edge cases in volume boolean operations?) so this is something to look out for should this be surprising.


## LICENSE

GPLv3

```
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
```

Source was based off of the `CSGBoolean` example which is licensed under `GPLv3`.


`mcut` is [licensed](https://github.com/cutdigital/mcut/blob/master/LICENSE.GPL.txt) under GPLv3.
