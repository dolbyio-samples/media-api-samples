#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>


#include <curl/curl.h>
#include <include/base64.hpp>
#include "curl_operations.h"
#include "dolbyio_calls.h"

#define API_KEY "replace_with_your_key"
#define API_SECRET "replace_with_your_secret_key"

int main()
{
    //filepath from opening file
    file_path = file.getFullPathName(); //JUCE function to get file path
    str_file_path = file_path.toUTF8(); //turning file path into a std::string
        
    //authenticate dolby.io
    std::string token = authenticate_dolbyio(API_KEY, API_SECRET, "10000");
        
    //upload file to dolby.io
    std::string job_url = upload_job_url(token);
        
    //putting the file into the job URL
    upload_file(str_file_path, job_url);
        
    sleep(5); //waits 5 seconds to allow time for processing
                
    //enhance file
    std::string dlb_job_id = enhance_dolbyio(token);
        
        //loop to check enhance status until it is done
    std::string dlb_job_status;
    while(1){
        sleep(1);
        dlb_job_status = "";
        dlb_job_status = enhance_status(token, dlb_job_id);
            
        if(dlb_job_status == ":\"Success"){
            break;
        } //end if
    } //end while
        
    //download newly enhanced file
    download_file(token, str_file_path);
        
  
  return 0;  
} //end main
