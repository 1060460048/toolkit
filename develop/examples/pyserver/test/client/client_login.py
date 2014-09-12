#!/bin/env python
# -*- coding: utf-8 -*-

"""
[pro@WSCENTOS64_x64 23:18:28 client]$time python client_login.py
time= 1410535123.16
time= 1410535125.7

real    0m2.604s
user    0m0.078s
sys     0m0.383s
"""

import socket
import struct
import json
import time
from threading import Thread
from multiprocessing import Process

HEAD_LEN=4

def Run():
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
#        print 'recved:',json.loads(body)

    print 'time=',time.time()
    sock.close()

    
# 多线程
#l = [ Thread(target = Run) for i in xrange(0,4) ]
# 多进程
l = [ Process(target = Run) for i in xrange(0,1) ]
for i in l:
    i.start()
for i in l:
    i.join()


