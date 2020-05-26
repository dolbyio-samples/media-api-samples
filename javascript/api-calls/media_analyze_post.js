#!/usr/bin/env node

const axios = require('axios').default;

const config = {
    method: 'post',
    url: 'https://api.dolby.com/media/analyze',
    headers: {
        'x-api-key': process.env.DOLBYIO_API_KEY,
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    },
    data: {
        input: 'dlb://in/example.mp3'
    }
};

axios(config)
    .then(function (response) {
        console.log(response.data.job_id);
    })
    .catch(function (error) {
        console.log(error);
    });

