#!/bin/bash

cd build.linux
make clean
make
cd ../test
make clean
make fileIO_test1
make fileIO_test2
# ../build.linux/nachos -d c -e fileIO_test1
# ../build.linux/nachos -d c -e fileIO_test2
../build.linux/nachos -e fileIO_test1
../build.linux/nachos -e fileIO_test2

# make consoleIO_test3
# ../build.linux/nachos -d c -e consoleIO_test3