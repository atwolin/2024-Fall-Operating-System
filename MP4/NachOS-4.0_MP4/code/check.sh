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

# make hw3t1 hw3t2 hw3t3
# ../build.linux/nachos -d z -ep hw3t1 100 -ep hw3t2 100 -ee
# ../build.linux/nachos -ep hw3t1 100 -ep hw3t2 100 -ee
# ./hw3_partA.sh # With nachos outputs
# ./hw3_all.sh # Without nachos outputs, verifies correctness

make FS_test1 FS_test2

# FS_partII_a.sh
# ../build.linux/nachos -f
# ../build.linux/nachos -mkdir /t0
# ../build.linux/nachos -cp FS_test1 /FS_test1
# ../build.linux/nachos -l /
# ../build.linux/nachos -r /FS_test1
# ../build.linux/nachos -l /
# ../build.linux/nachos -e /FS_test1
# ../build.linux/nachos -p /file1
# ../build.linux/nachos -cp FS_test2 /FS_test2
# ../build.linux/nachos -e /FS_test2


# FS_partII_b.sh
# ../build.linux/nachos -f
# ../build.linux/nachos -cp num_1000.txt /1000
# ../build.linux/nachos -p /1000

# FS_partIII.sh
../build.linux/nachos -f
../build.linux/nachos -mkdir /t0
../build.linux/nachos -mkdir /t1
../build.linux/nachos -cp num_100.txt /t0/f1
../build.linux/nachos -l /t0
../build.linux/nachos -r /t0/f1
../build.linux/nachos -l /
../build.linux/nachos -mkdir /t0/aa
../build.linux/nachos -cp num_100.txt /t0/aa/f1
../build.linux/nachos -l /
../build.linux/nachos -l /t0
../build.linux/nachos -l /t1
../build.linux/nachos -l /t0/aa
../build.linux/nachos -r /t0/aa/f1
../build.linux/nachos -l /t0

# echo ===================
# ../build.linux/nachos -lr /

# Bonus II
# ../build.linux/nachos -f
# ../build.linux/nachos -cp num_200.txt /200
# ../build.linux/nachos -cp num_800.txt /800
# ../build.linux/nachos -cp num_2000.txt /2000