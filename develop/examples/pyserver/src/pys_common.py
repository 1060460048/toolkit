#!/bin/env python
# -*- coding: utf-8 -*-

import pymysql
from gevent import monkey
monkey.patch_socket() 
from gevent.queue import Queue
import gevent


PYS_SERVICE_MOD_PRE='pys_'  # 模块名称的前缀
HEAD_LEN=4
MAX_BODY_LEN=40960
    
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


class SimpleDBPool():
    HOSTS='192.168.17.153'
    PASSWD='tm'
    USER='root'
    DB='test'
    def __init__(self):
        self._dbFree =Queue()
        for conn in [pymysql.connect(host=self.HOSTS,passwd=self.PASSWD,user=self.USER,db=self.DB) for i in xrange(0,10)] :
            self._dbFree.put(conn)
        
    def Get(self):
        conn =self._dbFree.get()
        return conn
    def Release(self,conn):
        self._dbFree.put(conn)
        gevent.sleep(0)        
pys_dbpool =SimpleDBPool()

