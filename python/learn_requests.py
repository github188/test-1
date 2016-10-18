#!/usr/bin/env python3.5
# -*- coding: utf-8 -*-

import requests

r = requests.get('http://git.io/17AROg')
print(r.text)
