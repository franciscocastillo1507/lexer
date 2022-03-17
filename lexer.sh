#!/bin/bash

apt-get install g++

echo "First arg: $1"

g++ ./lexico.cpp $1

./a.out
