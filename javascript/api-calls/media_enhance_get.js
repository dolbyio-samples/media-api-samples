#!/usr/bin/env node

const axios = require('axios').default;

const config = {
    method: 'get',
    url: 'https://api.dolby.com/media/enhance',
    headers: {
        'x-api-key': process.env.DOLBYIO_API_KEY,
        'Content-Type': 'application/json',
        'Accept': 'application/json'
    },
    params: {
        job_id: process.env.DOLBYIO_JOB_ID
    }
};

axios(config)
    .then(function (response) {
        console.log(JSON.stringify(response.data, null, 4));
    })
    .catch(function (error) {
        console.log(error);
    });

