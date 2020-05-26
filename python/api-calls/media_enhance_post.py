#!/usr/bin/env python

import os
import requests

url = "https://api.dolby.com/media/enhance"
headers = {
    "x-api-key": os.environ["DOLBYIO_API_KEY"],
    "Content-Type": "application/json",
    "Accept": "application/json",
}

body = {
    "input": os.environ["DOLBYIO_INPUT"],
    "output": "dlb://out/example-enhanced.mp3",
}

response = requests.post(url, json=body, headers=headers)
response.raise_for_status()
print(response.json()["job_id"])
