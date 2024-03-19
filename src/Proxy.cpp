#include "Proxy.h"
Proxy::Proxy(string proxy) { this->changeProxy(proxy); }

Proxy::~Proxy() {}

void Proxy::changeProxy(string proxy) {
  size_t protocol_end = proxy.find("://");
  if (protocol_end != string::npos) {
    this->protocol = proxy.substr(0, protocol_end);
    size_t ip_start = protocol_end + 3;
    size_t ip_end = proxy.find(':', ip_start);
    if (ip_end != string::npos) {
      this->ip = proxy.substr(ip_start, ip_end - ip_start);
      size_t port_start = ip_end + 1;
      this->port = stoi(proxy.substr(port_start));
    }
  } else {
    cout << RED << "Error al inicializar proxy: " << YELLOW << proxy << RESET
         << endl;
  }
}

bool Proxy::isWorking() {
  CURL *ch;
  CURLcode res;
  ch = curl_easy_init();
  if (ch) {
    curl_easy_setopt(ch, CURLOPT_URL, "http://google.com/");
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, true);
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, 15);
    curl_easy_setopt(ch, CURLOPT_USERAGENT,
                     "Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 5.1; "
                     "InfoPath.2; SLCC1; .NET CLR 3.0.4506.2152; .NET CLR "
                     "3.5.30729; .NET CLR 2.0.50727)2011-09-08 13:55:49");

    curl_easy_setopt(ch, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
    if (this->protocol == "socks5") {
      curl_easy_setopt(ch, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
    } else if (this->protocol == "socks4") {
      curl_easy_setopt(ch, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS4);
    }

    curl_easy_setopt(ch, CURLOPT_PROXY, ip.c_str());
    curl_easy_setopt(ch, CURLOPT_PROXYPORT, port);
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, &buffer);
    res = curl_easy_perform(ch);
    curl_easy_cleanup(ch);
    if (res == CURLE_OK) {
      if (sizeof(buffer) > 0)
        return true;
      else
        return false;
    }
  }
  return false;
}

std::string Proxy::toString() {
  std::string result =
      this->protocol + "://" + this->ip + ":" + std::to_string(this->port);
  return result;
}

size_t Proxy::WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}
