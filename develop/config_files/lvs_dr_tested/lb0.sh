#!/bin/sh

service iptables stop

vip=192.168.0.249
rs0=192.168.0.56
rs1=192.168.0.57

case "$1" in
start)
	echo "start lvs"
	/sbin/ifconfig eth0:0 $vip broadcast $vip netmask 255.255.255.255 up
	/sbin/route add -host $vip dev eth0:0
	
	/sbin/ipvsadm -C
	
	/sbin/ipvsadm -A -t $vip:8600 -s rr
	/sbin/ipvsadm -A -t $vip:8600 -r $rs0:8600 -g
	/sbin/ipvsadm -A -t $vip:8600 -r $rs1:8600 -g
	
	/sbin/ipvsadm
	
	echo "1" > /proc/sys/net/ipv4/ip_forward
	echo "1" > /proc/sys/net/ipv4/conf/all/send_redirects
	echo "1" > /proc/sys/net/ipv4/conf/default/send_redirects
	echo "1" > /proc/sys/net/ipv4/conf/eth0/send_redirects
	
	/sbin/ipvsadm --set 12 18 120
;;
    stop)
	echo "stop lvs"
	/sbin/ipvsadm -C
	/sbin/ifconfig eth0:0 down
;;

*)

echo "Usage : $0 {start|stop}"
exit 1
esac



	
	
	
	
	
	