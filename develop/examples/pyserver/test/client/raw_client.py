#!/bin/env python
# -*- coding: utf-8 -*-

"""
[tm@localhost client]$ time python raw_client.py
time= 1410507223.82
time= 1410507237.32

real    0m13.570s
user    0m0.420s
sys     0m0.459s
"""

import socket
import struct
import json
import time

HEAD_LEN=4

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('127.0.0.1',18600))

print 'time=',time.time()

u = u'zhangsan'
s = u.encode('UTF-8')

data =json.dumps({'cmd':'login','user':s,'pwd':'12345'})
data =struct.pack('!i',len(data))+data

total =4000

n =0
while n<total:
    n +=1
    sock.sendall(data)
#    print 'sent: n=%d,data=%s'%(n,data[4:])

    header =sock.recv(HEAD_LEN)
    bodyLen =struct.unpack('!i',header)[0]
    body =sock.recv(bodyLen)
#    print 'recved:',json.loads(body)

print 'time=',time.time()
sock.close()

