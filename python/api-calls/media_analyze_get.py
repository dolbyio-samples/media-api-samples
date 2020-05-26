#!/usr/bin/env python

import os
import requests
from pprint import pprint

job_id = os.environ["DOLBYIO_JOB_ID"]

url = "https://api.dolby.com/media/analyze"
headers = {
    "x-api-key": os.environ["DOLBYIO_API_KEY"],
    "Content-Type": "application/json",
    "Accept": "application/json",
}

params = {
    "job_id": job_id,
}

response = requests.get(url, params=params, headers=headers)
response.raise_for_status()
pprint(response.json())
