#!/usr/bin/env python

import os
import requests

input_path = os.environ["INPUT_MEDIA_LOCAL_PATH"]

url = "https://api.dolby.com/media/input"
headers = {
    "x-api-key": os.environ["DOLBYIO_API_KEY"],
    "Content-Type": "application/json",
    "Accept": "application/json",
}

body = {
    "url": "dlb://in/example.mp3",
}

response = requests.post(url, json=body, headers=headers)
response.raise_for_status()
data = response.json()
presigned_url = data["url"]
print("Uploading {0} to {1}".format(input_path, presigned_url))
with open(input_path, "rb") as input_file:
    requests.put(presigned_url, data=input_file)
