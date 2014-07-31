#!/bin/sh

g++ -Wall -g redis-binary-tool.cpp -lhiredis -Lhiredis-master -o redis-binary-tool
