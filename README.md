# synthetic-things

[![synthetic-things](https://snapcraft.io/synthetic-things/badge.svg)](https://snapcraft.io/synthetic-things)

A fantastic factory to make synthetic fruits.

## Build
- build on ubuntu.

_install dependencies first_

~~~
sudo apt install libasound2-dev libglu1-mesa-dev libpng-dev libx11-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev libxcursor-dev
~~~

clone and build

~~~
git clone https://github.com/Joker2770/synthetic-things.git --recursive
git submodule update --init --recursive
cd synthetic-things
mkdir build
cd build
cmake ..
make
~~~

- build on Windows.

~~~
git clone https://github.com/Joker2770/synthetic-things.git --recursive
git submodule update --init --recursive
cd synthetic-things/msvc
~~~

build with visual studio.

## Thanks

* [Synthetic-Game](https://github.com/YYYCZ/Synthetic-Game)

