#!/usr/bin/env python3.5
# -*- coding: utf-8 -*-

import json
import requests

#url_login = 'https://192.168.25.46/index.php/welcome/login'
#para = {'username':'admin', 'password':'sundray@123', 'times':'0'}
url_login = 'https://192.168.25.46/index.php/market/login'
para = {'market_usr':'admin', 'market_pwd':'sundray@123', 'times':'0'}

s = requests.Session()

r = s.post(url_login, data = para, verify=False, allow_redirects = False)
print(r.request.headers)
print(r.headers)
cookies = dict(sf_session_market=r.cookies['sf_session_market'])
print(cookies)
print("\n\n\n\n\n\n\n")

url_login = 'https://192.168.25.46/WLAN/market.php'

r = s.get(url_login, verify=False)#, cookies=cookies)
print(r.request.headers)
print(r.headers)
cookies = dict(sf_session_market=r.cookies['sf_session_market'])
print(cookies)
num = r.text.index('var token = "')
session_token = r.text[num+13:num+45]
print("\n\n\n\n\n\n\n")

url_data = 'https://192.168.25.46/market.php/market_runtime'
para = {'opr' : "poll"}
headers = {'Content-Type' : 'application/json',
        'X-Requested-With' : 'XMLHttpRequest',
        'Referer' : 'https://192.168.25.46/WLAN/market.php'
        }
headers['session_token'] = session_token

r = s.post(url_data, data=json.dumps(para), verify=False, headers=headers)
print(r.request.headers)
print(r.headers)
print(r.text.encode('latin-1').decode('unicode_escape'))
print("\n\n\n\n\n\n\n")

url_data = 'https://192.168.25.46/index.php/new_guest_analyse'
para = {"opr":"guestAns","start":0,"limit":3,"time_type":"last7day","type":"AP组","name":"/全部","id":-1,"branch_id":0}
headers['session_token'] = session_token

r = s.post(url_data, data=json.dumps(para), verify=False, headers=headers)
print(r.request.headers)
print(r.headers)
print(r.text.encode('latin-1').decode('unicode_escape'))
print('\n\n\n\n')
json_r = r.text.encode('latin-1').decode('unicode_escape')
print(json.dumps(json_r))
