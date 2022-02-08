# How to Use the Transcode API Postman Collection

You can use Postman to try out the Transcode API. Postman is a widely used platform for API development and testing.

## You Will Need

- Postman - If you haven't already, install the Postman application [here](https://www.postman.com/downloads/).
- Media API Key (find this on your dashboard on the [Dolby.io website](https://dolby.io/))
- An accessible URL of the file you wish to process. If you do not have this, see [I/O Collection Tutorial](docs/How-to-Use-Enhance-Collection.md).

### Start Transcoding

> This is the call that will initiate the transcoding of your uploaded file.

> If you wish you can utilize the [API Documentation on Dolby.io](https://docs.dolby.io/media-apis/reference/media-transcode-post) to change the parameters, which you can read more about in the [documentation](https://docs.dolby.io/media-apis/docs/transcode-api-guide).

1. Modify the parameters in **Transcode (BETA) -> Variables** to configure your API call.

2. Click the **"Send"** button to start the enhancement of your file

- If you receive an error message, make sure the previous steps were completed correctly & that you have a valid request Body.
- If the call is successful, the return status will be `200 OK` and you will see a `job_id` returned in the response body window. Move onto "Get Transcode Status"

Example Result:

```json
{
  "api_version": "v1.0",
  "result": {
    "version": "1.0.0"
  },
  "path": "/media/transcode",
  "status": "Success",
  "error": {
    "type": "string",
    "title": "string",
    "detail": "string"
  },
  "progress": 100
}
```

### Get Transcode Status

> This GET call will poll the returned `job_id` from the previous step to check the job status. Once the job is complete & successful, you will need to download the output file using the [I/O Collection Tutorial](docs/How-to-Use-Enhance-Collection.md).

1. In this last request, simply click the **"Send"** button again to poll the status of the job kicked-off in the previous step. If the job is not yet complete, you will see the following in the response body:

_Example Response for Job in Progress_

```json
{
    "path": "/media/transcode",
    "status": "Running",
    "progress": <Progress_Value>,
    "api_version": <API_Version>
}
```

- If you receive an error, check the received error code against the reference listed in the API docs.

To download the final output one the status is finished, see [I/O Collection Tutorial](docs/How-to-Use-Enhance-Collection.md).
