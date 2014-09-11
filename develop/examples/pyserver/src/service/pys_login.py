#!/bin/env python
# -*- coding: utf-8 -*-
    
import json

def Entry(req):
    
    rsp =json.dumps({'cmd':'_loginrsp','rt':'1','desc':req['user']+' login ok.'})    
    return rsp
