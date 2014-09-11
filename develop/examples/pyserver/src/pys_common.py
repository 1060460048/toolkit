#!/bin/env python
# -*- coding: utf-8 -*-

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


import pymysql
from gevent import monkey
monkey.patch_socket() 
from gevent.queue import Queue

class SimpleDBPool():
    HOST='localhost'
    PASSWD='pwd'
    USER='root'
    DB='test'
    def __init__(self):
        self._dbFree =Queue()
        for conn in [pymysql.connect(host=HOST,passwd=PASSWD,user=USER,db=DB) for i in xrange(0,10)] :
            self._dbFree.put(conn)
        print dbFreeQueue
        
    def Get():
        conn =self.dbFreeQueue.get()
        return conn
    def Release(conn):
        dbFreeQueue.put(conn)
        gevent.sleep(0)
        
// TODO:
def dbQuery1(sql,db): 
    print '---- dbQuery1 start ----'
    cursor = db.cursor() 
    data = cursor.execute(sql) 
    cursor.close() 
    db.commit()
    for e in cursor:
        print e 
    print '---- dbQuery1 over ----'
        
