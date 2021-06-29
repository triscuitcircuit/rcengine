#!/bin/bash
echo building Xcode
cd ..
mkdir xcode-build
cd xcode-build || exit
cmake -G Xcode ..
