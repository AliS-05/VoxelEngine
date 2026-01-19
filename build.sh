#!/bin/bash
g++ main.cpp -o game -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
if [ $? -eq 0 ]; then
    ./game
fi
