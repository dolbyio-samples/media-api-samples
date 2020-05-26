#!/usr/bin/env python

"""

This sample demonstrates a media workflow implemented as a command line script.  Given
a media file passed in on the command line the script will upload it to Dolby.io and
then analyze it to print out useful information.

# Example Usage

You will need to have a Python 3 runtime environment along with dependencies mentioned in
requirements.txt installed.  You pass the input file as an option to the command.

    $ python media_analyze.py wow_30s.mp3 --key <your-dolby-api-key>
    Uploaded file to location: dlb://in/analyze
    Job running with job_id: 6900019a-7523-49bf-b999-9f3213630ad2
    .
    .
    {
        "path": "/media/analyze",
        "progress": 100,
        "result": {
            "audio": {
                "bandwidth": 7235,
                "clipping": {
                    "first_event": 0,
                    "num_events": 0
                },
                ...
    }

If you don't want to provide your `--key` on each run you can also set it to be an environment
variable called `DOLBYIO_API_KEY`.  You'll need to use a command appropriate for your computer's
shell environment.  For example, in `bash` this would look like:

    $ export DOLBYIO_API_KEY="your-dolby-api-key"

# Customization

There are some additional parameters you can use with the APIs to customize how it
operates.  Review the API reference and change the `body` dictionary in the code
below.

For example, you could change it to:

    body = {
        'input': input_url,
        'content': {
            'type': 'podcast'
        },
        'loudness': {
            'profile': 'service_spotify'
        }
    }

# Need Help

Visit https://support.dolby.io and create a ticket.

"""

import os
import time
import json
import logging
import argparse
import requests


class App:
    def __init__(self):
        self.parser = argparse.ArgumentParser(description="sample dolby.io app for analyzing media file")
        self.parser.add_argument("input_file", help="local file path for an input file")
        self.parser.add_argument("--info", help="INFO debugging output", action="store_true")
        self.parser.add_argument("--debug", help="DEBUG output", action="store_true")
        self.parser.add_argument("--key", help="Dolby.io Media Processingn API Key", default="")
        self.parser.add_argument("--wait", help="Seconds to wait in between status checks", default=5)

        self.args = self.parser.parse_args()

        if self.args.info:
            logging.basicConfig(level=logging.INFO)

        if self.args.debug:
            logging.basicConfig(level=logging.DEBUG)

        self.args.key = self.args.key or os.environ.get("DOLBYIO_API_KEY")
        if not self.args.key:
            raise ValueError("Use --key or set environment variable DOLBYIO_API_KEY")

    def _get_url(self, path):
        return "https://api.dolby.com/" + path

    def _get_headers(self):
        return {
            "x-api-key": self.args.key,
            "Content-Type": "application/json",
            "Accept": "application/json",
        }

    def post_media_input(self, input_path, name):
        url = self._get_url("/media/input")
        headers = self._get_headers()
        body = {
            "url": name,
        }
        response = requests.post(url, json=body, headers=headers)
        response.raise_for_status()
        data = response.json()
        presigned_url = data["url"]
        logging.info("Uploading {0} to {1}".format(input_path, presigned_url))
        with open(input_path, "rb") as input_file:
            requests.put(presigned_url, data=input_file)

    def post_media_analyze(self, input_url):
        url = self._get_url("/media/analyze")
        headers = self._get_headers()
        # **CUSTOMIZATION**
        # If you want to change the behavior of the analyze process you
        # can add parameters found in the API reference to this body.
        # https://dolby.io/developers/media-processing/api-reference/analyze
        body = {"input": input_url}

        response = requests.post(url, json=body, headers=headers)
        response.raise_for_status()
        return response.json()["job_id"]

    def get_media_analyze(self, job_id):
        url = self._get_url("/media/analyze")
        headers = self._get_headers()
        params = {"job_id": job_id}

        response = requests.get(url, params=params, headers=headers)
        response.raise_for_status()
        data = response.json()
        if data["status"] not in {"Pending", "Running"}:
            return data
        # Keep on retrying untill job is complete
        logging.debug(data["status"])
        print(".")
        time.sleep(self.args.wait)
        return self.get_media_analyze(job_id)

    def main(self):
        # First we upload the input file to /media/input
        dlb_in = "dlb://in/analyze"
        print("Uploading file to location: {}".format(dlb_in))
        self.post_media_input(self.args.input_file, dlb_in)
        print("Uploaded file to location: {}".format(dlb_in))

        # Next, we start /media/analyze to begin analysis
        job_id = self.post_media_analyze(dlb_in)
        print("Job running with job_id: {}".format(job_id))

        # We need to check for results of the analysis and display results
        # to the terminal.
        results = self.get_media_analyze(job_id)
        print(json.dumps(results, indent=4, sort_keys=True))


if __name__ == "__main__":
    App().main()
