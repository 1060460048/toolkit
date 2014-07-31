#!/bin/sh

service iptables stop

VIP=192.168.1.200

case "$1" in
start)
    /bin/echo "start LVS of TUN"
    /sbin/ifconfig tunl0 $VIP broadcast $VIP netmask 255.255.255.255 up
    /sbin/route add -host $VIP dev tunl0
    # /bin/echo "1" > /proc/sys/net/ipv4/conf/lo/arp_ignore
    # /bin/echo "2" > /proc/sys/net/ipv4/conf/lo/arp_announce
    /bin/echo "0" > /proc/sys/net/ipv4/ip_forward
    /bin/echo "1" > /proc/sys/net/ipv4/conf/tunl0/arp_ignore
    /bin/echo "2" > /proc/sys/net/ipv4/conf/tunl0/arp_announce
    /bin/echo "0" > /proc/sys/net/ipv4/conf/tunl0/rp_filter
    /bin/echo "1" > /proc/sys/net/ipv4/conf/all/arp_ignore
    /bin/echo "2" > /proc/sys/net/ipv4/conf/all/arp_announce
    /bin/echo "0" > /proc/sys/net/ipv4/conf/all/rp_filter
    echo "RealServer Start OK"
;;
stop)
    /bin/echo "stop LVS of TUN"
    /sbin/ifconfig tunl0 down
    # /bin/echo "0" > /proc/sys/net/ipv4/conf/lo/arp_ignore
    # /bin/echo "0" > /proc/sys/net/ipv4/conf/lo/arp_announce
    /bin/echo "0" > /proc/sys/net/ipv4/ip_forward
    /bin/echo "0" > /proc/sys/net/ipv4/conf/all/arp_ignore
    /bin/echo "0" > /proc/sys/net/ipv4/conf/all/arp_announce
    /bin/rm -Rf /proc/sys/net/ipv4/conf/all/rp_filter
    /bin/echo "RealServer Stoped"
;;
*)

echo "Usage:$0 {start|stop}"

exit 1
esac

