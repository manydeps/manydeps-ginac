#!/bin/bash
# THIS SCRIPT GOES INSIDE external/cln FOLDER
(mkdir -p build && cd build && cmake .. --preset=linux-release && ninja)
