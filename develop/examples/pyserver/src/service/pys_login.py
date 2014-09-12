#!/bin/env python
# -*- coding: utf-8 -*-
    
import json
import sys
import time
sys.path.append('../')
from pys_common import *
    

def tic(s, start):
    print '%s, %1.6f '%(s,(time.time() - start)*1000)
    
def db_checkuser(db,user): 
    cursor = db.cursor() 
    sql ="select * from t where name='%s'"%(user)
    data = cursor.execute(sql) 
    cursor.close()     
#    for e in cursor:
#        print e 
    rt =False
    if cursor.rowcount >0:
        rt =True
    return rt
    
def Entry(req):
#    return json.dumps({'cmd':'_loginrsp','rt':'1','desc':req['user']+' login succ.'})

    rsp =None
    db =pys_dbpool.Get()  
    try:
        rt ='0'
        desc =req['user']+' login failed.'
        if db_checkuser(db,req['user']):
            rt ='1'
            desc =req['user']+' login ok.'
        rsp =json.dumps({'cmd':'_loginrsp','rt':rt,'desc':desc})  
    except Exception,e:
        print e
        db.rollback()
    else :
        db.commit()
    pys_dbpool.Release(db)
    return rsp
