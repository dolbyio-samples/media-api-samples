#!/usr/bin/env python

import os
import shutil
import requests

url = 'https://api.dolby.com/media/output'
headers = {
    'x-api-key': os.environ['DOLBYIO_API_KEY'],
    'Content-Type': 'application/json',
    'Accept': 'application/json'
}

args = {
    'url' : 'dlb://out/example-enhanced.mp3',
}

response = requests.get(url, params=args, headers=headers, stream=True)
if response.status_code == 200:
    with open(os.environ['YOUR_OUTPUT_PATH'], 'w+b') as file_content:
        response.raw.decode_content = True
        shutil.copyfileobj(response.raw, file_content)

