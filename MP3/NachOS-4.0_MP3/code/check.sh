#!/bin/bash

cd build.linux
make clean
make
cd ../test
# make clean
# make fileIO_test1
# make fileIO_test2
# ../build.linux/nachos -d c -e fileIO_test1
# ../build.linux/nachos -d c -e fileIO_test2
# ../build.linux/nachos -e fileIO_test1
# ../build.linux/nachos -e fileIO_test2

# make consoleIO_test3
# ../build.linux/nachos -d c -e consoleIO_test3

make clean
# make consoleIO_test1 consoleIO_test2
# ../build.linux/nachos -d t -e consoleIO_test2
# ../build.linux/nachos -d a -e consoleIO_test1
# ../build.linux/nachos -d t -e consoleIO_test1 -d t -e consoleIO_test2
# ../build.linux/nachos -e consoleIO_test1 -e consoleIO_test2

make hw3t1 hw3t2 hw3t3
../build.linux/nachos -d z -ep hw3t1 100 -ep hw3t2 100 -ee
# ../build.linux/nachos -ep hw3t1 100 -ep hw3t2 100 -ee
# ./hw3_partA.sh # With nachos outputs
./hw3_all.sh # Without nachos outputs, verifies correctness