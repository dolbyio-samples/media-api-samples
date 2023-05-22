
# Cpp

To use the cpp headers you will need a base64 library and a wrapper to use cURL in cpp.

This is the base64 library used in this repo: https://github.com/tobiaslocker/base64

For curl wrappers, there are many available on github, use whichever works best for your application.


## Workflow

### media_enhance.cpp

This workflow script:

- uses /media/input to upload a local file
- uses /media/enhance to process your media
- uses /medaia/output to download the resulting media to local filesystem

uses the default parameters but can be modified as needed using the settings found in the documentation.


## API Calls

This is a collection of short header files that allow the user to call functions that perform cURL requests. 

Currently, the file /dolbyio_calls.h only supports /media/input, /media/output, and /media/enhance.

