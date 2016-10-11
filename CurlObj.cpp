//
// Created by Rutvik Choudhary on 10/6/16.
//

#include "CurlObj.h"


size_t writeCallback(void* contents, size_t size, size_t nmemb, void* buffer)
{
    ((std::string*) buffer)->append((char*) contents, size * nmemb);
    return size * nmemb;
}

// Creates the handle and sets some basic options
CurlObj::CurlObj()
{
    hnd = curl_easy_init();
    curl_easy_setopt(hnd, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writeCallback);
}

CurlObj::~CurlObj()
{
    curl_easy_cleanup(hnd);
}

void CurlObj::getWebpage(const std::string& url, std::string& buffer)
{
    curl_easy_setopt(hnd, CURLOPT_URL, url.c_str());
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &buffer);
    buffer.clear();
    buffer.reserve(300000);
    curl_easy_perform(hnd);
}