#!/bin/sh
# Athor
if [ $# -lt 4 ] ; then
    echo "Usage:  $0 <username> <password> <host> <database>"
    exit 0
fi

U=$1
P=$2
H=$3
DB=$4

mysql -u$U -p$P -h$H $DB < ~/update_tm_userface.sql 
mysql -u$U -p$P -h$H $DB -e "call update_tm_userface( 0, 1, 47 );";
mysql -u$U -p$P -h$H $DB -e "DROP PROCEDURE IF EXISTS insert_group_member_by_groupnum";

echo 'Run completely.'
