#!/bin/bash

apt-get install g++

echo "First arg: $1"

g++ ./lexico.cpp

sudo ./a.out $1
