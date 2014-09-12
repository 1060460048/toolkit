#!/bin/env python
# -*- coding: utf-8 -*-


import socket
import struct
import json
import time

HEAD_LEN=4

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(('127.0.0.1',18600))

print 'time=',time.time()


data =json.dumps({u'cmd':u'echo',u'data':u'hello,world!'})
data =struct.pack('!i',len(data))+data

total =1

n =0
while n<total:
    n +=1
    sock.sendall(data)
    print 'sent: n=%d,data=%s'%(n,data[4:])

    header =sock.recv(HEAD_LEN)
    bodyLen =struct.unpack('!i',header)[0]
    body =sock.recv(bodyLen)
#    print 'recved:',json.loads(body)

print 'time=',time.time()
sock.close()

