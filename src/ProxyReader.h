#pragma once
#include "Proxy.h"
#include <queue>

class ProxyReader {
public:
  ProxyReader();
  ~ProxyReader();

  void readProxies(string path);
  Proxy giveNext();

private:
  queue<Proxy> proxies;
};
