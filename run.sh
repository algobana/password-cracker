#!/bin/bash

OUTDIR="logs"
BIN="./cracker_v4"
BIN_GPROF="./cracker_v4_gprof"

mkdir -p $OUTDIR

echo "=== time ==="
/usr/bin/time -v $BIN &> "$OUTDIR/time.txt"

echo "=== valgrind ==="
valgrind --leak-check=full --log-file="$OUTDIR/valgrind.txt" $BIN

echo "=== gprof ==="
g++ -pg -std=c++17 -Wall -O3 -pthread -o $BIN_GPROF main.cpp
$BIN_GPROF
gprof $BIN_GPROF gmon.out > "$OUTDIR/gprof.txt"

echo "=== perf stat ==="
$BIN &                      
PID=$!
sleep 1
perf stat -p $PID -o "$OUTDIR/perf.txt" sleep 5
wait $PID

echo "=== top ==="
top -b -n 1 > "$OUTDIR/top.txt"



echo "=== mpstat ==="
mpstat -P ALL 1 3 > "$OUTDIR/mpstat.txt"

echo "=== sar ==="
sar -u 1 3 > "$OUTDIR/sar.txt"

echo "All logs saved in $OUTDIR/"

