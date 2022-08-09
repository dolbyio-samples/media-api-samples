/*
  ==============================================================================

    curl_operations.h
    Created: 12 Jul 2022 9:15:56am
    Author:  Bennett, E

  ==============================================================================
*/

#pragma once

#include <stdio.h>
#include <string.h>
#include <iostream>

#include <curl/curl.h>
#include <include/base64.hpp>

//==========================================================================
size_t write_func(void *ptr, size_t size, size_t nmemb, std::string *s)
{
    s->append(static_cast<char *>(ptr), size* nmemb);
    fprintf(stderr, "*** We wrote %lu bytes to string\n", nmemb);
    return size* nmemb;
}

//==========================================================================
size_t write_to_file(void* contents, size_t size, size_t nmemb, void* stream)
{
    size_t written = fwrite(contents, size, nmemb, (FILE *)stream);
    fprintf(stderr, "*** We wrote %lu bytes to file\n", written);
    
    return written;
} //It was originally published on https://www.apriorit.com/

//==========================================================================
static size_t read_callback(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t retcode;
    unsigned long nread;
 
    /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */
    
    retcode = fread(ptr, size, nmemb, stream);

 
    if(retcode > 0) {
        nread = (unsigned long)retcode;
        fprintf(stderr, "*** We read %lu bytes from file\n", nread);
    }
 
    return retcode;
}

//==========================================================================
std::string parse_token(std::string s, int occur)
{
    //parse json to get what I need
    std::string parsed;
    char parser[] = "\""; // looking for "
    size_t start_location = 0;
    size_t end_location = 0;

    int count = 0;
    while (count < occur){ //finds third " which is right before the token
        start_location = s.find(parser, start_location + count);
        count++;
    }
    
    end_location = s.find(parser, start_location + count);
    long length_token = end_location - start_location; //length of substring
    
    parsed = s.substr(start_location+1, length_token-1); //getting token from json
    
    return parsed; //returning bearer token from auth process
}
