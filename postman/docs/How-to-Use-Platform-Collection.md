# How to Use the Transcode API Postman Collection

You can use Postman to try out the Platform API. Postman is a widely used platform for API development and testing.

## You Will Need

- Postman - If you haven't already, install the Postman application [here](https://www.postman.com/downloads/).
- Media API Key and Secret (find this on your dashboard on the [Dolby.io website](https://dolby.io/))
- A local audio file to upload

### Generate OAuth2 Token

> This is the call that will create an OAuth2 Token for authentication using your API key and secret.

1. Input your Media API Secret in **Platform -> Variables** and save.

2. Click the **"Send"** button to generate your Bearer Token.

- If you receive an error message, make sure the previous steps were completed correctly & that you have a valid request Body.
- If the call is successful, the return status will be `200 OK` and you will see a `job_id` returned in the response body window. Move onto "Enhance GET Result"

Sample Response:

```json
{
  "access_token": "aSAmpleAcESSt0ken",
  "expires_in": 43199,
  "token_type": "Bearer"
}
```

### Media Jobs

> This GET call will return a list of all jobs submitted in the past 31 days. Configure optional parameters seen in the [API Reference](https://docs.dolby.io/media-apis/reference/media-jobs-get).

1. In this request, simply click the **"Send"** button to collect the response.

_Example Response for Jobs_

```json
{
  "jobs": [
    {
      "job_id": "082bfe89-eb23-442e-b6d3-54d1b3b01632",
      "api_version": "v1.1",
      "path": "/media/enhance",
      "status": "Success",
      "progress": 100,
      "duration": 40.12,
      "time_submitted": "2021-05-24T12:57:56.602Z",
      "time_started": "2021-05-24T12:59:56.602Z",
      "time_completed": "2021-05-24T13:05:12.502Z",
      "expiry": "2021-06-24T12:59:56.602Z"
    }
  ],
  "next_token": "eyJ0aW1lU3VibWl0dGVkIjoiMjAyMS0wNC0yMFQwMjowMDozOS4zODBaIiwiam9iSWQiOiJlNGNmNzA2Zi01MWYyLTQ1NDctODY0Zi0wN2M5N2E5OTg5ZDQiLCJhcHBJZCI6IlRFU1RfTE9DQUxfdG1penUifQ==",
  "count": 100
}
```

- If you receive an error, check the received error code against the reference listed in the API docs.

### Cancel a Job

> This POST request will cancel a previous submitted job in progress.

1. In this request, simply click the **"Send"** button to send the request.

_Example Response for Cancel Job_

```json
{
  "status": 200,
  "title": "The cancellation request is received and cancellation of the job will be attempted",
  "detail": "Your cancel request was successfully submitted. Check the status of the job for further information."
}
```

### Register Webhook

> This POST request will create a new webhook for an API key only if one does not already exist. Otherwise use Update Webhook.

1. Add the callback URL under \*\* **Platform -> Variables** and save.

2. Click **Send**.

_Example Response for Cancel Job_

```json
{
  "webhook_id": "webhook-6ee70181d63da278b47560a544fac083"
}
```

### Update Webhook

> This PUT request updates an existing webhook.

1. Add the webhook ID and the new callback URL under \*\* **Platform -> Variables** and save.

2. Click **Send**.

_Example Response for Cancel Job_

```json
{
  "webhook_id": "webhook-6ee70181d63da278b47560a544fac083"
}
```

### Retrieve Webhook

> This GET request retrieves the existing webhook configuration.

1. Add the webhook ID under \*\* **Platform -> Variables** and save.

2. Click **Send**.

_Example Response for Cancel Job_

```json
[
  {
    "webhook_id": "webhook-6ee70181d63da278b47560a544fac083",
    "callback": {
      "url": "string",
      "headers": {
        "additionalProp": "string"
      }
    }
  }
]
```

### Delete Webhook

> This DELETE request deletes an existing webhook.

1. Add the webhook ID under \*\* **Platform -> Variables** and save.

2. Click **Send**.

_Example Response for Cancel Job_

```json
{
  "webhook_id": "webhook-6ee70181d63da278b47560a544fac083"
}
```
