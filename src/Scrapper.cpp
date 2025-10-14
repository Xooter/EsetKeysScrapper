#include "Scrapper.h"

Scrapper::Scrapper() {
  this->curl = curl_easy_init();

  if (!this->curl) {
    throw " Error al inicializar CURL";
  }

  curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYPEER, 0L);
  curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYHOST, 0L);
  curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &this->response);
  curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, WriteCallback);

  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, handleHeaders);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, &responseHeaders);

  curl_easy_setopt(this->curl, CURLOPT_ENCODING, "UTF-8");
  curl_easy_setopt(this->curl, CURLOPT_ACCEPT_ENCODING, "UTF-8");
  curl_easy_setopt(this->curl, CURLOPT_TIMEOUT, 15);
}

Scrapper::~Scrapper() { curl_easy_cleanup(this->curl); }

size_t Scrapper::WriteCallback(void *contents, size_t size, size_t nmemb,
                               void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

size_t Scrapper::handleHeaders(char *buffer, size_t size, size_t nitems,
                               std::string *output) {
  size_t totalSize = size * nitems;
  output->append(buffer, totalSize);
  return totalSize;
}

std::string Scrapper::pickUserAgent() {

  int index = rand() % this->userAgents.size();
  return "User-Agent: " + this->userAgents[index];
}

void Scrapper::setHeaders() {
  if (!this->curl) {
     std::cerr << "[ERROR] curl handle is null!" << std::endl;
     return;
  }
  headers = curl_slist_append(headers, this->pickUserAgent().c_str());

  if (this->token != "") {
    string tokenHeader = "Authorization: Bearer " + this->token;
    headers = curl_slist_append(headers, tokenHeader.c_str());
  }
  curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);
}
