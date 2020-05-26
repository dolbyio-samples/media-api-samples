#!/usr/bin/env python

import os
import shutil
import requests

output_path = os.environ["OUTPUT_MEDIA_LOCAL_PATH"]

url = "https://api.dolby.com/media/output"
headers = {
    "x-api-key": os.environ["DOLBYIO_API_KEY"],
    "Content-Type": "application/json",
    "Accept": "application/json",
}

args = {
    "url": "dlb://out/example-enhanced.mp3",
}

with requests.get(url, params=args, headers=headers, stream=True) as response:
    response.raise_for_status()
    response.raw.decode_content = True
    print("Downloading from {0} into {1}".format(response.url, output_path))
    with open(output_path, "wb") as output_file:
        shutil.copyfileobj(response.raw, output_file)
