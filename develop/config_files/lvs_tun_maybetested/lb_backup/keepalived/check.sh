#!/bin/bash

LOGFILE="/etc/keepalived/log.txt"

ALIVE=`/bin/cat /etc/keepalived/tst`
if [ "$ALIVE" == "1" ]; then
  echo "[alive]" >> $LOGFILE
  exit 0
else
  echo "[not alive]" >> $LOGFILE
  exit 1
fi

