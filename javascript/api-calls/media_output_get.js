#!/usr/bin/env node

const fs = require('fs');
const axios = require('axios').default;

const output_path = process.env.OUTPUT_MEDIA_LOCAL_PATH;

const config = {
    method: 'get',
    url: 'https://api.dolby.com/media/output',
    headers: {
        'x-api-key': process.env.DOLBYIO_API_KEY,
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    },
    responseType: 'stream',
    params: {
        url: 'dlb://out/example-enhanced.mp3'
    }
};

axios(config)
    .then(function (response) {
        response.data.pipe(fs.createWriteStream(output_path));
        response.data.on('error', function (error) {
            console.log(error);
        });
        response.data.on('end', function () {
            console.log('File downloaded!');
        });
    })
    .catch(function (error) {
        console.log(error);
    });
