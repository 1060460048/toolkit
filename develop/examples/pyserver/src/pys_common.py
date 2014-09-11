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

