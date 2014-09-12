#!/bin/env python
# -*- coding: utf-8 -*-
    
import gevent
from gevent.server import StreamServer
from gevent.pool import Pool
from gevent.queue import Queue
from gevent import monkey
monkey.patch_socket()

import struct
import time
import sys
import json

from pys_common import *
sys.path.append('./service')

def TaskWorker(taskQueue,id):
    while True:
        task = taskQueue.get()
#        print'Worker %d got task %r ' % (id,task)
        if task.has_key('stop'):
            break

        if task.has_key('sock') and task.has_key('req'):
            req =json.loads(task['req'])
            modname =PYS_SERVICE_MOD_PRE+req['cmd']
            if not sys.modules.has_key(modname):
                __import__(modname)
            
            rsp =sys.modules[modname].Entry(req)
            if rsp:
                rspPacket =struct.pack('!i',len(rsp))+rsp
                task['sock'].sendall(rspPacket)
#                print 'sent,size=%d,%s'%(len(rsp),rsp)
            else : # no rsp to send.
                pass
        else :
            print 'error of task:,',task
            
        gevent.sleep(0)

def handler(sock, address):
    print('New connection from %s:%s' % address)

    taskQueue =Queue()
    taskWorker =gevent.spawn(TaskWorker,taskQueue,sock.fileno())
    
    while True:
        header = recvall(sock,HEAD_LEN)
        if not header:
            break
        bodyLen =struct.unpack('!i',header)[0]
        if bodyLen > MAX_BODY_LEN :
            print 'bodyLen=',bodyLen
            break;
        elif bodyLen > 0 :
            req = recvall(sock,bodyLen)
#            print 'recved:size=%d,%s'%(len(req),req)
            if not header:  # error.
                break
            
            taskQueue.put({'sock':sock,'req':req})
    taskQueue.put({'stop':''})
    sock.close()
    gevent.joinall([taskWorker])
    
if __name__ == '__main__':
    server = StreamServer(('0.0.0.0', 18600), handler, backlog=512, spawn=Pool(5000))
    print 'main starting'
    server.serve_forever()

