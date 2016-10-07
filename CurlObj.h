//
// Created by Rutvik Choudhary on 12/15/15.
//

#pragma once

#include <string>
#include <curl/curl.h>
#include <iostream>

// The callback function passed to cURL so that we can store the retrieved contents
// in a buffer
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* buffer);

// Used to encapsulate the setup of a cURL handle
class CurlObj
{
private:
    CURL* hnd;

public:
    CurlObj();
    ~CurlObj();

    // Sets some other options before retrieving the webpage at `url` and storing the contents into
    // a user provided `std::string`
    void getWebpage(const std::string& url, std::string& buffer);
};