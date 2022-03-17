#!/bin/sh

mkdir -p build && cd build || exit

cmake -DLIBOBS_INCLUDE_DIR="../$1" -DCMAKE_INSTALL_PREFIX=/usr ..
make -j4

# Exit if nothing was built (no code changed since last compilation).
if [ ! -d rundir ]; then exit; fi

# Moves plugin folder to root of build folder.
if [ -d zls-plugin ]; then
  rm -rf zls-plugin/*
else
  mkdir zls-plugin
fi

mv rundir/zls-plugin/* zls-plugin/
rm -rf rundir
