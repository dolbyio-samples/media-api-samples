
# Python

To run the Python samples you need to install a Python 3 environment and install the
libraries referenced in requirements.txt.

You may find these resources helpful:

* https://docs.python-guide.org/starting/installation/

## Workflows

### media_analyze.py

This workflow script:

- uses /media/input to upload a local file
- uses /media/analyze to print results of analyzing media to STDOUT

It uses just the default parameters so requires modification to
use other settings found in the documentation.

### media_enhance.py

This workflow script:

- uses /media/input to upload a local file
- uses /media/enhance to process your media
- uses /media/output to download the resulting media to local filesystem

It uses just the default parameters so requires modification to
use other settings found in the documentation.

## API Calls

This is a collection of short snippets that demonstrate get and post calls
to each Media Processing API.

