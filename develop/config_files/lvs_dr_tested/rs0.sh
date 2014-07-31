#!/bin/sh

service iptables stop

vip=192.168.0.249

case "$1" in 
start)
	echo "start lvs of rs $vip"
	/sbin/ifconfig lo:0 $vip broadcast $vip netmask 255.255.255.255 up
	/sbin/route add -host $vip dev lo:0
	
	echo "1" > /proc/sys/net/ipv4/ip_forward
	echo "1" > /proc/sys/net/ipv4/conf/lo/arp_ignore
	echo "2" > /proc/sys/net/ipv4/conf/lo/arp_announce
	echo "1" > /proc/sys/net/ipv4/conf/all/arp_ignore
	echo "2" > /proc/sys/net/ipv4/conf/all/arp_announce

	sysctl -p
;;
stop)
	echo "stop lvs of rs $vip"
	/sbin/ifconfig lo:0 down
	
	echo "0" > /proc/sys/net/ipv4/conf/lo/arp_ignore
	echo "0" > /proc/sys/net/ipv4/conf/lo/arp_announce
	echo "0" > /proc/sys/net/ipv4/conf/all/arp_ignore
	echo "0" > /proc/sys/net/ipv4/conf/all/arp_announce
;;
*)

echo "Usage: $0 {start|stop}"
exit 1
esac
	
	
	
	
