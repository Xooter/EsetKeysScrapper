#include "ProxyReader.h"
ProxyReader::ProxyReader() {}

ProxyReader::~ProxyReader() {}

void ProxyReader::readProxies(string path) {
  ifstream file(path);

  if (!file.is_open()) {
    cerr << RED << " Failed to open file: " << YELLOW << path << RESET
         << endl;
  }

  string line;
  while (getline(file, line)) {
    proxies.emplace(line);
  }

  file.close();
}

Proxy ProxyReader::giveNext() {
  if (proxies.empty()) {
    cerr << RED << " No working proxies available..." << RESET << endl;
    cerr << RED << "Leaving the program" << RESET << endl;
  }
  Proxy proxy = proxies.front();
  proxies.pop();
  return proxy;
}
