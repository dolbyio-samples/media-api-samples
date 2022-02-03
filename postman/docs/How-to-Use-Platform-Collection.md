# How to Use the Transcode API Postman Collection

You can use Postman to try out the Platform API. Postman is a widely used platform for API development and testing.

## You Will Need

- Postman - If you haven't already, install the Postman application [here](https://www.postman.com/downloads/).
- API Key (find this on your dashboard on the [Dolby.io website](https://dolby.io/))
- A local audio file to upload

### Enhance POST

> This is the call that will initiate the enhancement of your uploaded file.

> If you wish you can utilize the [API Documentation on Dolby.io](https://dolby.io/developers/media-processing/api-reference/analyze) to change the parameters

1. Click the **"Send"** button to start the enhancement of your file

- If you receive an error message, make sure the previous steps were completed correctly & that you have a valid request Body.
- If the call is successful, the return status will be `200 OK` and you will see a `job_id` returned in the response body window. Move onto "Enhance GET Result"

### Enhance GET Result

> This GET call will poll the returned `job_id` from the previous step to check the job status. Once the job is complete & successful, the response body will contain the JSON results of the file analysis.

1. In this last request, simply click the **"Send"** button again to poll the status of the job kicked-off in the previous step. If the job is not yet complete, you will see the following in the response body:

_Example Response for Job in Progress_

```
{
    "path": "/media/enhance",
    "status": "Running",
    "progress": <Progress_Value>,
    "api_version": <API_Version>
}
```

- If you receive an error, check the received error code against the reference listed in the API docs.

### Downloading the Output File

1. Click the GET Download Output Request

2. _Instead_ of clicking "Send", click on the arrow to the right of "Send", then select **"Send and Download"** from the dropdown

<img src="./images/send_and_download.png" width=300>

3. If the request is successful, a Save File window will open in your file manager

4. The default file extenstion is `*.bin`, so make sure to manually assign the same file extension as your uploaded input file (ex: change "response.bin" to  "enhanced_sample_file.wav")

<img src="./images/save_output.png" width=400>

5. You can now open your process file wherever you saved it in your file system and hear the playback
