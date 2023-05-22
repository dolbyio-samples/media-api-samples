/*
  ==============================================================================

    dolbyio_calls.h
    Created: 28 Jul 2022 7:45:33am
    Author:  Bennett, E
 
    This has functions to all users to run the enhance API in cURL using C++
    external libraries required: cURL for cpp and base64
 
    cURL is used to make rest API calls from c++
    base64 is used to encode the bearer token

  ==============================================================================
*/

#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>

#include <curl/curl.h>
#include <include/base64.hpp>


std::string authenticate_dolbyio(std::string API_KEY, std::string API_SECRET, std::string expires)
/*used to make an authentication token to make calls to dolby.io
 std::string API_KEY            found on user dashboard under 'Get API Keys' - this is the 'App Key'
 std::string API_SECRET         found on user dashboard under 'Get API Keys' - this is the 'App Secret'
 std::string expires            value from 0 - 2,592,000 (in seconds), how long the token will be valid for
 */
{
    //variables needed
    std::string auth_token;
    std::string post_field;
    std::string bearer_token;
    
    //authentication of dolby.io
    CURL *auth = curl_easy_init(); //initialize instance of cURL
    CURLcode result; //will store result from post
    
        //encoding the auth. token
    auth_token = API_KEY + ":" + API_SECRET;
    auto encoded = base64::to_base64(auth_token); //encoding with base64

        //strings needed for header / postfields in cURL post request
    auth_token = "Authorization: Basic " + encoded; //used in header later
    post_field = "grant_type=client_credentials&expires_in=" + expires; //used in postfield later

    //start cURL request
    curl_easy_setopt(auth, CURLOPT_CUSTOMREQUEST, "POST"); //option is POST
    curl_easy_setopt(auth, CURLOPT_URL, "https://api.dolby.io/v1/auth/token"); //location to POST to

        //start cURL header
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Cache-Control: no-cache");
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    headers = curl_slist_append(headers, reinterpret_cast<const char*>(auth_token.c_str())); //auth
    curl_easy_setopt(auth, CURLOPT_HTTPHEADER, headers); //end header

        //post header field - grant_type
    curl_easy_setopt(auth, CURLOPT_POSTFIELDS, "grant_type=client_credentials&expires_in=10000");

        //writing results of post to a variable (holding token for later)
    curl_easy_setopt(auth, CURLOPT_WRITEFUNCTION, write_func);
    curl_easy_setopt(auth, CURLOPT_WRITEDATA, &bearer_token);

        //getting result and closing the cURL instance
    std::cout << "========== RESULTS OF AUTH ==========" << std::endl;
    result = curl_easy_perform(auth); //checking for errors by storing json result
    curl_easy_cleanup(auth); //closes cURL after use
    bearer_token = parse_token(bearer_token, 3); //parsing auth token from json !!

    std::cout << "bearer token: " << bearer_token << std::endl;
    std::cout << "\n\n" << std::endl;
    
    return bearer_token;
} //end authenticate_dolbyio
//----------------------------------------------------------------------------------------------------------------

std::string upload_job_url(std::string token)
/*used to make an upload url to upload a file to
  std::string token      returned value from authenticate_dolbyio
 */
{
    //variables needed
    std::string auth_token;     //used to authenticate dolby.io in header
    std::string dlb_input_url;  //will hold full json response from post request
    std::string dlb_job_url;    //will hold parsed json response from post request
    
    const char* file_up_char;
    
    CURL *upload = curl_easy_init();
    CURLcode result; //will store result from post
    
    //making input file url for dolby.io
    file_up_char = "{\"url\": \"dlb://input/file.wav\"}";
    
    //updating auth token header field
    auth_token = "Authorization: Bearer " + token; //used in header later
    
    //setting up post request
    curl_easy_setopt(upload, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(upload, CURLOPT_URL, "https://api.dolby.com/media/input");
    
      //headers for post
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, reinterpret_cast<const char*>(auth_token.c_str()));
    headers = curl_slist_append(headers, "x-internal-request: true");
    curl_easy_setopt(upload, CURLOPT_HTTPHEADER, headers);
    
      //data for post
    curl_easy_setopt(upload, CURLOPT_POSTFIELDS, file_up_char);
    
      //writing results of post to a variable (holding upload url for later)
    curl_easy_setopt(upload, CURLOPT_WRITEFUNCTION, write_func);
    curl_easy_setopt(upload, CURLOPT_WRITEDATA, &dlb_input_url);
    
      //getting result and closing the cURL instance
    std::cout << "========== RESULTS OF UPLOAD ==========" << std::endl;
    result = curl_easy_perform(upload);
    curl_easy_cleanup(upload);
    
    dlb_job_url = parse_token(dlb_input_url, 3); //parsing job url from json !!
    std::cout << "upload job url: " << dlb_job_url << std::endl;
    std::cout << "\n\n" << std::endl;
    
    return dlb_job_url;
} //end upload_job_url
//----------------------------------------------------------------------------------------------------------------

void upload_file(std::string file_path, std::string job_url)
/* used to upload a file to the upload url
   std::string file_path     local file path of file you want to upload
   std::string job_url        returned value from upload_job_url
 */
{
    //variables needed
    FILE * enhance_file;
    
    //opening file
    enhance_file = fopen(static_cast<const char*>(file_path.c_str()), "rb");
    
    //finding file size
    fseek(enhance_file, 0L, SEEK_END);
    curl_off_t curl_size = ftell(enhance_file);
    fseek(enhance_file, 0L, SEEK_SET);
    
    //setting up cURL
    CURL *cont_upload = curl_easy_init();
    CURLcode result; //will store result from post
    
    //setting up UPLOAD request
    curl_easy_setopt(cont_upload, CURLOPT_READFUNCTION, read_callback); //setting up to read
    curl_easy_setopt(cont_upload, CURLOPT_UPLOAD, 1L); //upload to url
    curl_easy_setopt(cont_upload, CURLOPT_URL, reinterpret_cast<const char*>(job_url.c_str())); //url
    curl_easy_setopt(cont_upload, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(cont_upload, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(cont_upload, CURLOPT_READDATA, enhance_file); //read from this file
    curl_easy_setopt(cont_upload, CURLOPT_INFILESIZE_LARGE, curl_size); //file size
    
      //getting result and closing the cURL instance
    std::cout << "========== RESULTS OF UPLOAD PT2 ==========" << std::endl;
    result = curl_easy_perform(cont_upload); //perform read and upload
    curl_easy_cleanup(cont_upload);
    std::cout << "\n\n" << std::endl;
    
    fclose(enhance_file); //close file when done
} //end upload_file
//----------------------------------------------------------------------------------------------------------------

std::string enhance_dolbyio(std::string token)
/* used to upload a file to the upload url
   std::string token     returned value from authenticate_dolbyio
 */
{
    //variables needed
    std::string enhance_data;   //used to make post fields for enhance post
    std::string dlb_job_url;    //stores full post return from enhance
    std::string dlb_job_id;     //stores parsed job id to check enhance status
    
    std::string auth_token;     //used to authenticate dolby.io in header
    
    const char* post_data;
    
    //updating auth token header field
    auth_token = "Authorization: Bearer " + token; //used in header later
    
    //setting up cURL instance
    CURL *enhance = curl_easy_init();
    CURLcode result; //will store result from post
    
    //setting up POST request
    curl_easy_setopt(enhance, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(enhance, CURLOPT_URL, "https://api.dolby.com/media/enhance");
    
        //headers for POST
    struct curl_slist *headers = NULL;
    headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, reinterpret_cast<const char*>(auth_token.c_str()));
    headers = curl_slist_append(headers, "x-internal-request: true");
    curl_easy_setopt(enhance, CURLOPT_HTTPHEADER, headers);
    
        //data for post
    enhance_data = "{\"input\": \"dlb://input/file.wav\"";
    enhance_data = enhance_data + ",\"output\":\"dlb://out/file.wav\"";
    enhance_data = enhance_data + ",\"content\": {\"type\": \"speech\"}}";
    
    std::cout << enhance_data << std::endl;
    
    post_data = static_cast<const char*>(enhance_data.c_str());
    
    curl_easy_setopt(enhance, CURLOPT_POSTFIELDS, post_data);
    
        //writing job ID to be able to check it later
    curl_easy_setopt(enhance, CURLOPT_WRITEFUNCTION, write_func);
    curl_easy_setopt(enhance, CURLOPT_WRITEDATA, &dlb_job_url);
    
        //getting result and closing the cURL instance
    std::cout << "========== RESULTS OF ENHANCE ==========" << std::endl;
    result = curl_easy_perform(enhance);
    curl_easy_cleanup(enhance);
    
    std::cout << "unparsed: " << dlb_job_url << std::endl;
    dlb_job_id = parse_token(dlb_job_url, 3); //parsing job url from json !!
    std::cout << "enhance job ID: " << dlb_job_id << std::endl;
    std::cout << "\n\n" << std::endl;
    
    return dlb_job_id;
} //end enhance_dolbyio
//----------------------------------------------------------------------------------------------------------------

std::string enhance_status(std::string token, std::string job_id)
/* used to evaluate if enhance is done running or not
   std::string token       value returned from authenticate_dolbyio
   std::string job_id      value returned from enhance_dolbyio
 */
{
    //variables needed
    std::string status_url;     //url to check for status
    std::string job_status;     //full json return
    std::string job_progress;   //parsed status report
    
    std::string auth_token;     //used to authenticate dolby.io in header
    
    //updating auth token header field
    auth_token = "Authorization: Bearer " + token; //used in header later

    //setting up cURL
    CURL *status = curl_easy_init();
    CURLcode result; //will store result from post

        //setting up GET request
    status_url = "https://api.dolby.com/media/enhance?job_id=" + job_id;
    curl_easy_setopt(status, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(status, CURLOPT_URL, reinterpret_cast<const char*>(status_url.c_str()));
    curl_easy_setopt(status, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(status, CURLOPT_DEFAULT_PROTOCOL, "https");

        //headers for GET
    struct curl_slist *headers = NULL;
    headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, reinterpret_cast<const char*>(auth_token.c_str()));
    headers = curl_slist_append(headers, "x-internal-request: true");
    curl_easy_setopt(status, CURLOPT_HTTPHEADER, headers);

        //writing job ID to be able to check it later
    curl_easy_setopt(status, CURLOPT_WRITEFUNCTION, write_func);
    curl_easy_setopt(status, CURLOPT_WRITEDATA, &job_status);

        //getting result and closing the cURL instance
    std::cout << "========== ENHANCE JOB STATUS ==========" << std::endl;
    result = curl_easy_perform(status);
    curl_easy_cleanup(status);
    std::cout << job_status << std::endl;
    std::cout << "\n\n" << std::endl;
    
    job_progress = parse_token(job_status, 5);
    std::cout << "progress amount: " << job_progress << std::endl;
    
    return job_progress;
} //end enhance_status
//----------------------------------------------------------------------------------------------------------------

void download_file(std::string token, std::string file_path)
/* used to download a file after processing is done
   std::string token       value returned from authenticate_dolbyio
   std::string file_path   location you want the file to be downloaded to
 */
{
    //variables needed
    FILE *fp;
    
    std::string output_url;     //url for cURL request to download from
    std::string new_file_name;  //name of new file that is being made from downloaded data
    
    std::string auth_token;     //used to authenticate dolby.io in header
    
    //updating auth token header field
    auth_token = "Authorization: Bearer " + token; //used in header later
    
    //setting up cURL
    CURL *download = curl_easy_init();
    CURLcode result;
    
        //setting up cURL request
    output_url = "https://api.dolby.com/media/output?url=dlb://out/file.wav";
    
    new_file_name = file_path + "-enhanced.wav"; //tacks id onto file_path to make clear file is enhanced
    
        //opening file and writing to it
    fp = fopen(reinterpret_cast<const char*>(new_file_name.c_str()), "wb");
    curl_easy_setopt(download, CURLOPT_URL, reinterpret_cast<const char*>(output_url.c_str()));
    curl_easy_setopt(download, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(download, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(download, CURLOPT_FOLLOWLOCATION, 1L);
    
        //headers for reqeust
    struct curl_slist *headers = NULL;
    headers = NULL;
    headers = curl_slist_append(headers, reinterpret_cast<const char*>(auth_token.c_str()));
    headers = curl_slist_append(headers, "x-internal-request: true");
    curl_easy_setopt(download, CURLOPT_HTTPHEADER, headers);
    
        //writing data into new file
    curl_easy_setopt(download, CURLOPT_WRITEFUNCTION, write_to_file);
    
    if(fp){ //error catch in case file doesn't open
        curl_easy_setopt(download, CURLOPT_WRITEDATA, fp);
    
        //getting result and closing the cURL instance
        std::cout << "========== RESULTS OF DOWNLOAD ==========" << std::endl;
        result = curl_easy_perform(download);
        curl_easy_cleanup(download);
        fclose(fp);
        std::cout << "\n\n" << std::endl;
    } //end if
    else{
        std::cout << "error downloading file" << std::endl;
    } //end else
  
} //end download_file
