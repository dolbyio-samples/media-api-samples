#!/usr/bin/env node

const fs = require('fs');
const axios = require('axios').default;

const file_path = process.env.INPUT_MEDIA_LOCAL_PATH;

const config = {
    method: 'post',
    url: 'https://api.dolby.com/media/input',
    headers: {
        'x-api-key': process.env.DOLBYIO_API_KEY,
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    },
    data: {
        url: 'dlb://in/example.mp3'
    }
};

axios(config)
    .then(function (response) {
        const upload_config = {
            method: 'put',
            url: response.data.url,
            data: fs.createReadStream(file_path),
            headers: {
                'Content-Type': 'application/octet-stream',
                'Content-Length': fs.statSync(file_path).size
            }
        };
        axios(upload_config)
            .then(function () {
                console.log("File uploaded")
            })
            .catch(function (error) {
                console.log(error);
            });
    })
    .catch(function (error) {
        console.log(error);
    });
