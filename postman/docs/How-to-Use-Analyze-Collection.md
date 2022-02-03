# How to Use the Analyze API Postman Collection

You can use Postman to try out the Analyze API. Postman is a widely used platform for API development and testing.

## You Will Need

- Postman - If you haven't already, install the Postman application [here](https://www.postman.com/downloads/).
- API Key (find this on your dashboard on the [Dolby.io website](https://dolby.io/))
- An accessible URL of the file you wish to process. If you do not have this, see [I/O Collection Tutorial](docs/How-to-Use-Enhance-Collection.md).

## Running the Analyze API

This will walk you through the steps of calling the Analyze API.

### Analyze

> This is the call that will initiate the analysis of your uploaded file.

> If you wish you can utilize the [API Documentation on Dolby.io](https://dolby.io/developers/media-processing/api-reference/analyze) to change the parameters

1. Click the **"Send"** button to start the analysis of your file

- If you receive an error message, make sure the previous steps were completed correctly & that you have a valid request Body.
- If the call is successful, the return status will be `200 OK` and you will see a `job_id` returned in the response body window. Move onto "Analyze GET Result"

### Analyze GET Result

> This GET call will poll the returned `job_id` from the previous step to check the job status. Once the job is complete & successful, the response body will contain the JSON results of the file analysis.

1. In this last request, simply click the **"Send"** button again to poll the status of the job kicked-off in the previous step. If the job is not yet complete, you will see the following in the response body:

_Example Response for Job in Progress_

```
{
    "path": "/media/analyze",
    "status": "Running",
    "progress": <Progress_Value>,
    "api_version": <API_Version>
}
```

_Example Response for Successful Job_

```
{
    "path": "/media/analyze",
    "status": "Success",
    "progress": 100,
    "api_version": "v1.0",
    "result": {
        "media_info": {
            "container": {
                "kind": "wav",
                "duration": 6.718666666666667,
                "bitrate": 768000,
                "size": 645072
            },
            "audio": {
                "codec": "pcm",
                "bit_depth": 16,
                "channels": 1,
                "sample_rate": 48000,
                "duration": 6.718666666666667,
                "bitrate": 768000
            }
        },
        "audio": {
            "clipping": {
                "num_events": 0,
                "first_event": 0
            },
            "loudness": {
                "measured": -20.45,
                "range": 4.49,
                "gating_mode": "speech",
                "sample_peak": -7.87,
                "true_peak": -7.86
            },
            "validation": {
                "loudness": {
                    "pass": false,
                    "detail": "measured loudness exceeds maximum specified (-20.45 > -22)"
                }
            },
            "silence": {
                "start": 0,
                "end": 0,
                "total": 0,
                "num_sections": 0
            },
            "noise": {
                "snr_average": 41.92,
                "level_average": -67.53
            },
            "bandwidth": 5062
        }
    }
}
```

- If you receive an error, check the received error code against the reference listed in the API docs.

To download the final output one the status is finished, see [I/O Collection Tutorial](docs/How-to-Use-Enhance-Collection.md).