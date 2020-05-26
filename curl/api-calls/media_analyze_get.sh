#!/bin/bash

curl -X GET "https://api.dolby.com/media/analyze?job_id=$DOLBYIO_JOB_ID" \
    --header "x-api-key: $DOLBYIO_API_KEY"
