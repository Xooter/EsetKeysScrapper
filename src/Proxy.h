#pragma once

#include "Helpers/Paths.h"

class Proxy {
public:
  std::string ip;
  int port;

  Proxy(std::string proxy);
  ~Proxy();

  void changeProxy(std::string proxy);
  bool isWorking();

  std::string toString();

private:
  std::string buffer;

  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);
};
