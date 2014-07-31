#!/bin/sh

cd ./src

g++ -Wall -g -fPIC -shared plug_a.cpp -olibplug_a.so
mv ./libplug_a.so ../bin
g++ -Wall -g -fPIC -shared plug_b.cpp -olibplug_b.so
mv ./libplug_b.so ../bin
g++ -Wall -g -fPIC -shared plug_c.cpp -olibplug_c.so
mv ./libplug_c.so ../bin

g++ -Wall -g server_main.cpp plugin_mgr.cpp -o server_main -ldl
mv ./server_main ../bin

cd ../

