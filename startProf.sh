#!/usr/bin/env bash

./fib_bench &
PID=$!

echo "running with PID: $PID" 

sudo sample $PID 8
