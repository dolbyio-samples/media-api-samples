// See following blog post for more detail:
// https://dolby.io/blog/getting-started-with-oauth2-for-media-processing-with-javascript

// Helper function to store OAuth 2 Cookie
function setCookie(name, value) {
  let maxAge = `;max-age=${12 * 60 * 60}`; // Expire cookie at 12 hours
  document.cookie =
    name + "=" + value + maxAge + ";path=/" + ";SameSite=Lax" + ";Secure";
}

// Helper function to get cookie value from key
function getCookie(name) {
  let value = "; " + document.cookie;
  let parts = value.split(`; ${name}=`);
  if (parts.length == 2) return parts.pop().split(";").shift();
}

// Function that takes in the base64 encoded parameters and create a JWT
function getToken(key, secret) {
  const params = new URLSearchParams();
  params.append("grant_type", "client_credentials");

  const config = {
    method: "post",
    url: "https://api.dolby.com/media/oauth2/token",
    headers: {
      // Convert key and secret to B64 encoding
      Authorization: `Basic ${btoa(`${key}:${secret}`)}`,
      "Content-Type": "application/x-www-form-urlencoded",
    },
    data: params,
  };

  axios(config)
    .then(function (response) {
      setCookie("token", response.data.access_token);
    })
    .catch(function (error) {
      console.log(error);
    });
}

// Check if non-expired cookie, otherwise prompts for input
document.addEventListener("DOMContentLoaded", function () {
  let token = getCookie("token");
  // Check to see if cookie exists
  if (!token) {
    let apiKey = prompt("Please enter Dolby.IO Media Processing API Key:");
    let secret = prompt("Please enter Dolby.IO Media Processing Secret:");
    getToken(apiKey, secret);
  }
});

// Create the optional cookie reset button
document.getElementById("resetToken").onclick = function () {
  setCookie("token", "");
};

// Sample API call using Token instead of API Key
function enhanceRequest(name) {
  // Create API call
  const config = {
    method: "post",
    url: "https://api.dolby.com/media/enhance",
    headers: {
      // Cookie authorization called here!
      Authorization: `Bearer ${getCookie("token")}`,
      "Content-Type": "application/json",
      Accept: "application/json",
    },
    data: {
      input: "<DOLBYIO_INPUT_URL>",
      output: "<DOLBYIO_OUTPUT_URL>",
    },
  };

  // Send API response
  axios(config)
    .then(function (response) {
      let message = { jobId: response.data.job_id, fileName: name };
      console.log(message);
    })
    .catch(function (error) {
      console.log(error);
    });
}
