#!/bin/bash

curl -X POST "https://api.dolby.com/media/analyze" \
    --header "x-api-key: $DOLBYIO_API_KEY" \
    --data '{
        "input": "dlb://in/example.mp3"
    }'
