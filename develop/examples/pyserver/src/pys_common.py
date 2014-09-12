#!/bin/env python
# -*- coding: utf-8 -*-

import pymysql
from gevent import monkey
monkey.patch_socket() 
from gevent.queue import Queue
import gevent
import struct
import sys


PYS_SERVICE_MOD_PRE='pys_'  # 模块名称的前缀
HEAD_LEN=4
MAX_BODY_LEN=40960
DBPOOL_SIZE=32
    
def recvall(sock,totalLen):
    data =''
    left =totalLen
    while left>0:
        d =sock.recv(left)
        if not d:
            data =None
            break
        data +=d
        left =totalLen-len(data)
    return data


def ImportModule(modname):
    if not sys.modules.has_key(modname):
        __import__(modname)
    return sys.modules[modname]

def CallModule(cmd,req):
    return ImportModule(PYS_SERVICE_MOD_PRE+cmd).Entry(req)
    
def PackData(rsp):
    return struct.pack('!i',len(rsp))+rsp
def UnpackData(header):
    return struct.unpack('!i',header)[0]
    
class SimpleDBPool():
    HOSTS='127.0.0.1'
    PASSWD='pwd'
    USER='root'
    DB='test'
    def __init__(self):
        self._dbFree =Queue()
        for conn in [pymysql.connect(host=self.HOSTS,passwd=self.PASSWD,user=self.USER,db=self.DB) for i in xrange(0,DBPOOL_SIZE)] :
            self._dbFree.put(conn)
        
    def Get(self):
        conn =self._dbFree.get()
        return conn
    def Release(self,conn,commit):
        if commit:
            conn.commit()
        else:
            conn.rollback()
        self._dbFree.put(conn)
        gevent.sleep(0)        
pys_dbpool =SimpleDBPool()

