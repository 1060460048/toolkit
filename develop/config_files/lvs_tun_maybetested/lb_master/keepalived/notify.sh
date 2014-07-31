#!/bin/bash 

vip=192.168.1.200 
contact='dungeonsnd@126.com'
 
notify() { 
    subject="$ip change to $1"
    body="$ip change to $1 $(date '+%F %H:%M:%S')"
    echo "$subject -- $body" >> /etc/keepalived/notify.txt
}
 
case "$1" in
master) 
notify master 
exit 0
;; 
backup) 
notify backup 
exit 0
;; 
fault) 
echo "fault detected... " >> /etc/keepalived/notify.txt
notify fault 
exit 0
;; 
stop) 
notify stop
exit 0
;;
*) 
echo 'Usage: $(basename $0) {master|backup|fault|stop}'
exit 1
;; 
esac 

