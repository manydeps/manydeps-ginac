#!/bin/bash

cp ../cln/build/include/cln/*.h ../cln/include/cln/
(mkdir -p build && cd build && cmake .. --preset=linux-release && ninja)
