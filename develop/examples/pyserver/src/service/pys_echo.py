#!/bin/env python
# -*- coding: utf-8 -*-
    
import json

def Entry(req):
    return json.dumps({'cmd':'_echorsp','data':req['data']})
