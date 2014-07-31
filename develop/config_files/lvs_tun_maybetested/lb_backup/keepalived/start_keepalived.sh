#!/bin/sh

setenforce 0
service iptables stop
/etc/init.d/keepalived start

