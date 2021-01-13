#!/bin/bash

# Pass the pre-signed url and local file on the commandline

curl -X PUT $1 -T $2
