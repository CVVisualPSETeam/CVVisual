#! /bin/bash

mkdir build
cd build


mkdir release
cd release
cmake  -DCMAKE_BUILD_TYPE=Release ../..
make -j$(nproc)
make test
cd ..

mkdir debug
cd debug
cmake  -DCMAKE_BUILD_TYPE=Debug ../..
make -j$(nproc)
make test
cd ..
