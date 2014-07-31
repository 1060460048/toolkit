#!/bin/sh

if [ $# -lt 3 ] ; then
    echo "Usage:  $0 <groupnum> <in_insert_count> <in_start_userid>"
    exit 0
fi

mysql -uroot -ptm -h192.168.17.153 tm3 < insert_group_member.sql 
mysql -uroot -ptm -h192.168.17.153 tm3 -e "call insert_group_member_by_groupnum( $1,$2,$3)";
mysql -uroot -ptm -h192.168.17.153 tm3 -e "DROP PROCEDURE IF EXISTS insert_group_member_by_groupnum"
mysql -uroot -ptm -h192.168.17.153 tm3 -e "DROP PROCEDURE IF EXISTS insert_group_member_by_groupid"
echo 'Run completely.'
