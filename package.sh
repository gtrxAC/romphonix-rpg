#!/bin/bash

rm -rf release
mkdir release

[[ -e bin/romphonix ]] || [[ -e bin/romphonix.exe ]] || ./build.sh

cp -r assets release/
cp -r bin release/
cp romphonix_linux.sh release/
cp romphonix_win.bat release/
cp README.md release/

cd release
rm -f assets/data/*.json assets/data/*.py assets/maps/*.h
cd ..