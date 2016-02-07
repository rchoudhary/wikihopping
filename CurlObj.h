//
// Created by Rutvik Choudhary on 12/15/15.
//

#pragma once

#include <string>
#include <curl/curl.h>
#include <iostream>

// The callback function passed to cURL so that we can store the retrieved contents
// in a buffer
size_t writeCallback(void* contents, size_t size, size_t nmemb, void* buffer)
{
    ((std::string*) buffer)->append((char*) contents, size * nmemb);
    return size * nmemb;
}

// Used to encapsulate the setup of a cURL handle
class CurlObj
{
private:
    CURL* hnd;

public:
    // Creates the handle and sets some basic options
    CurlObj()
    {
        hnd = curl_easy_init();
        curl_easy_setopt(hnd, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writeCallback);
    }

    ~CurlObj()
    {
        curl_easy_cleanup(hnd);
    }

    // Sets some other options before retrieving the webpage at `url` and storing the contents into
    // a user provided `std::string`
    void getWebpage(const std::string& url, std::string& buffer)
    {
        curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
        curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &buffer);
        buffer.clear();
        buffer.reserve(300000);
        curl_easy_perform(hnd);
    }
};