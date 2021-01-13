#!/usr/bin/env python

"""

This sample demonstrates a media workflow implemented as a command line script.  Given
a media file passed in on the command line the script will upload it to Dolby.io
then enhance it and download the result to disk.

# Example Usage

You will need to have a Python 3 runtime environment along with dependencies mentioned in
requirements.txt installed. You pass the input and output file as options to the command.

    $ python media_enhance.py wow_30s.mp3 wow_30s_enhanced.mp3 --key <your-dolby-api-key>
    Uploaded file to location: dlb://in/enhance
    Job running with job_id: 36cd9bec-1ab2-4717-b5b7-178ec257e9a6
    .
    .
    .
    .
    .
    .
    Job complete: {
        "path": "/media/enhance",
        "progress": 100,
        "result": {},
        "status": "Success"
    }
    Downloading file from location: dlb://out/enhance
    File created: wow_enhanced.mp3

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
        'output': output_url,
        'content': {
            'type': 'podcast'
        },
        'audio': {
            'noise': {
                'reduction': {
                    'amount': 'max'
                }
            }
        }
    }

# Need Help

Visit https://support.dolby.io and create a ticket.

"""

import os
import time
import json
import shutil
import logging
import argparse
import requests


class App:
    def __init__(self):
        self.parser = argparse.ArgumentParser(description="sample dolby.io app for enhancing media file")
        self.parser.add_argument("input_file", help="local file path for an input file")
        self.parser.add_argument("output_file", help="local file path for writing output file")
        self.parser.add_argument("--info", help="INFO debugging output", action="store_true")
        self.parser.add_argument("--debug", help="DEBUG output", action="store_true")
        self.parser.add_argument("--key", help="Dolby.io Media Processingn API Key", default="")
        self.parser.add_argument("--wait", help="Seconds to wait in between status checks", default=6)

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

    def post_media_enhance(self, input_url, output_url):
        url = self._get_url("/media/enhance")
        headers = self._get_headers()
        # Customization
        #
        # If you want to change the behavior of the enhance process you
        # can add parameters found in the API reference to this body.
        # https://dolby.io/developers/media-processing/api-reference/enhance
        # body = {"input": input_url, "output": output_url}
        body = {
                "input": input_url,
                "output": output_url,
                'content': {
                    'type': 'podcast'
                },
                'audio': {
                    'noise': {
                        'reduction': {
                            'amount': 'max'
                        }
                    }
                }
            }

        response = requests.post(url, json=body, headers=headers)
        response.raise_for_status()
        return response.json()["job_id"]

    def get_media_enhance(self, job_id):
        url = self._get_url("/media/enhance")
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
        return self.get_media_enhance(job_id)

    def get_media_output(self, dlb_out, output_path):
        url = self._get_url("/media/output")
        headers = self._get_headers()
        params = {"url": dlb_out}

        with requests.get(url, params=params, headers=headers, stream=True) as response:
            response.raise_for_status()
            response.raw.decode_content = True
            logging.info("Downloading from {0} into {1}".format(response.url, output_path))
            with open(output_path, "wb") as output_file:
                shutil.copyfileobj(response.raw, output_file)

    def main(self):
        # First we upload the input file to /media/input
        dlb_in = "dlb://in/enhance"
        print("Uploading file to location: {}".format(dlb_in))
        self.post_media_input(self.args.input_file, dlb_in)
        print("Uploaded file to location: {}".format(dlb_in))

        # Next, we start /media/enhance to begin enhancing
        dlb_out = "dlb://out/enhance"
        job_id = self.post_media_enhance(dlb_in, dlb_out)
        print("Job running with job_id: {}".format(job_id))

        # We need to check for results of the enhance process and display results
        # to the terminal.
        results = self.get_media_enhance(job_id)
        print("Job complete: {}".format(json.dumps(results, indent=4, sort_keys=True)))

        # When complete, we can download the result and save it locally
        print("Downloading file from location: {}".format(dlb_out))
        self.get_media_output(dlb_out, self.args.output_file)
        print("File available: {}".format(self.args.output_file))


if __name__ == "__main__":
    App().main()
