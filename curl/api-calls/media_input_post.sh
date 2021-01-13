#!/bin/bash

curl -X POST "https://api.dolby.com/media/input" \
    --header "x-api-key: $DOLBYIO_API_KEY" \
    --data '{
        "url": "dlb://in/example.mp3"
    }'
