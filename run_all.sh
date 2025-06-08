#!/bin/bash

mkdir -p logs

for level in O1 O2 O3
do
    echo "===> Building with -$level ..."
    make clean > /dev/null
    make rebuild OLEVEL=$level > /dev/null

    echo "===> Running with -$level ..."
    echo "Running cracker_v4 with -$level optimization..." > logs/opt_$level.txt
    /usr/bin/time -v ./cracker_v4 2>&1 | tee -a logs/opt_$level.txt

    echo "Output saved to logs/opt_$level.txt"
    echo ""
done

