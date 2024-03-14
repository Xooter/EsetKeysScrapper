#pragma once

#include "Helpers/Paths.h"

class Scrapper {
public:
  Scrapper();
  ~Scrapper();

private:
  string pickUserAgent();
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);

protected:
  CURL *curl;
  CURLcode code;
  struct curl_slist *headers = NULL;

  string response;
  string token;

  vector<std::string> userAgents{
      "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, "
      "like Gecko) Chrome/74.0.3729.169 Safari/537.36",
      "Mozilla/5.0 (iPhone; CPU iPhone OS 12_2 like Mac OS X) "
      "AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148",
      "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"};

  virtual void setHeaders();
};
