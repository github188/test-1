#!/usr/bin/env python3.5
# -*- coding: utf-8 -*-

import json
 
obj = [[1,2,3],123,123.123,'abc',{'key1':(1,2,3),'key2':(4,5,6)}]
encodedjson = json.dumps(obj)
print(repr(obj))
print(encodedjson)

buf = json.loads(encodedjson)
print(type(buf))
print(buf)

a = u'%fadsfdaasdsdfa'
print(a)

for i in range(1, 5):
    print(i)
