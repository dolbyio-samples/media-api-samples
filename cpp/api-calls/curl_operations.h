/*
  ==============================================================================

    curl_operations.h
    Used to read and write to files as well as write to strings. 
    Also allows parsing of json responses from cURL requests.
    
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
size_t write_func(void *buffer, size_t size, size_t memb, std::string *s)
{
    s->append(static_cast<char *>(buffer), size* memb);
    fprintf(stderr, "Wrote %lu bytes to string\n", memb);
    return size* memb;
}

//==========================================================================
size_t write_to_file(void *buffer, size_t size, size_t memb, void* stream)
{
    size_t bytes_written = fwrite(buffer, size, memb, (FILE *)stream);
    fprintf(stderr, "Wrote %lu bytes to file\n", bytes_written);
    
    return bytes_written;
}

//==========================================================================
static size_t read_callback(void *buffer, size_t size, size_t memb, FILE *stream)
{
    size_t bytes_read;
    unsigned long nread;
 
    /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */
    
    bytes_read = fread(buffer, size, memb, stream);

 
    if(bytes_read > 0) {
        nread = (unsigned long)bytes_read;
        fprintf(stderr, "Read %lu bytes from file\n", nread);
    }
 
    return bytes_read;
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
    while (count < occur){ //looks for " to find what is needed from different cURL requests
        start_location = s.find(parser, start_location + count);
        count++;
    }
    
    end_location = s.find(parser, start_location + count);
    long length_token = end_location - start_location; //length of substring
    
    parsed = s.substr(start_location+1, length_token-1); //parsing json
    
    return parsed; //returns parsed json
}
