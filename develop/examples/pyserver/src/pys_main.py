#!/bin/env python
# -*- coding: utf-8 -*-
    
import gevent
from gevent.server import StreamServer
from gevent.pool import Pool
from gevent.queue import Queue
from gevent import monkey
monkey.patch_socket()

from pys_common import *
import sys
sys.path.append('./service')

import time
import json

def DoTask(task):
    if task.has_key('sock') and task.has_key('req'):
        req =json.loads(task['req'])
        rsp =CallModule(req['cmd'],req)
        if rsp:
            task['sock'].sendall(PackData(rsp))
#                print 'sent,size=%d,%s'%(len(rsp),rsp)
        else : # no rsp to send.
            pass
    else :
        print 'error of task:,',task

def TaskWorker(taskQueue,id):
    while True:
        task = taskQueue.get()
#        print'Worker %d got task %r ' % (id,task)
        if task.has_key('stop'):
            break
        DoTask(task)            
        gevent.sleep(0)

def handler(sock, address):
    print('New connection from %s:%s' % address)

    taskQueue =Queue()
    taskWorker =gevent.spawn(TaskWorker,taskQueue,sock.fileno()) # 处理协程用于从任务队列中取出任务并处理.
    
    while True:
        header = recvall(sock,HEAD_LEN)
        if not header:
            break
        bodyLen =UnpackData(header)
        if bodyLen > MAX_BODY_LEN :
            print 'bodyLen=',bodyLen
            break;
        elif bodyLen > 0 :
            req = recvall(sock,bodyLen)
#            print 'recved:size=%d,%s'%(len(req),req)
            if not header:  # error.
                break
            
            task ={'sock':sock,'req':req}
#            DoTask(task)   # 测试接收和处理在同一协程中
            taskQueue.put(task)
#            print'Put task to worker %d , taskQueue.qsize=%d' % ( sock.fileno(), taskQueue.qsize())
    taskQueue.put({'stop':''})
    sock.close()
    gevent.joinall([taskWorker])
    
if __name__ == '__main__':
    server = StreamServer(('0.0.0.0', 18600), handler, backlog=512, spawn=Pool(5000))
    print 'main starting'
    server.serve_forever()

