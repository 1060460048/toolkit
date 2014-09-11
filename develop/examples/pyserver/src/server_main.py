#!/bin/env python
    
import gevent
from gevent.server import StreamServer
from gevent.pool import Pool
from gevent.queue import Queue
from gevent import monkey
monkey.patch_socket()
import struct
import time

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

def TaskWorker(taskQueue):
    while True:
        task = taskQueue.get()
#        print'Worker got task ' ,task
        if task.has_key('stop'):
            break

        if task.has_key('sock') and task.has_key('req'):
            rsp ='Server response->'+task['req']
            rspPacket =struct.pack('!i',len(rsp))+rsp
            task['sock'].sendall(rspPacket)
#            print 'sent',rsp
        else :
            print 'error of task:,',task
            
        gevent.sleep(0)

def handler(sock, address):
    print('New connection from %s:%s' % address)

    taskQueue =Queue()
    taskWorker =gevent.spawn(TaskWorker,taskQueue)
    
    HEAD_LEN=4
    MAX_BODY_LEN=10240
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
#            print 'recved:',req
            if not header:  # error.
                break
            
            taskQueue.put({'sock':sock,'req':req})
    taskQueue.put({'stop':''})
    sock.close()
    
if __name__ == '__main__':
    server = StreamServer(('0.0.0.0', 18600), handler, backlog=512, spawn=Pool(5000))
    server.serve_forever()

