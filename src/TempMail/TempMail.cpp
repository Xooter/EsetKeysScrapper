#include "TempMail.h"

TempMail::TempMail(int mailLenght) : Scrapper() {
  this->setHeaders();

  this->mailLenght = mailLenght;

  this->getNewEmail();
}

TempMail::~TempMail() {}

bool TempMail::getMessages() {
  this->response.clear();

  curl_easy_setopt(this->curl, CURLOPT_URL, GET_MESSAGES.c_str());
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK) {
    json jsonResponse = json::parse(this->response);
    for (auto &message : jsonResponse) {

      string id = message["id"];
      string from = message["from"]["address"];
      string subject = message["subject"];
      string body = message["intro"];

      this->messages.push_back({id, from, subject, body});
    }

    return true;
  }

  return false;
}

Message TempMail::readMessage(string id) {
  this->response.clear();

  string url = GET_MESSAGE + id;
  curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK) {
    json jsonResponse = json::parse(this->response);
    for (auto &mensaje : messages) {
      if (mensaje.id == id) {
        mensaje.read = true;
        mensaje.body = jsonResponse["text"];

        return mensaje;
      }
    }
  }

  return {};
}

bool TempMail::getNewEmail() {
  this->response.clear();

  string address;
  address = generateRandomAddress();

  json postData = {{"address", address}, {"password", PASSWORD}};

  string jsonBody = postData.dump();

  curl_easy_setopt(this->curl, CURLOPT_URL, CREATE_MAIL.c_str());
  curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
  curl_easy_setopt(this->curl, CURLOPT_POST, 1L);

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK) {
    if (this->response.find("violations") != string::npos) {
      ofstream existingAddressesFile(getPath() + EXISTING_ADDRESSES_FILE,
                                     ios::app);
      existingAddressesFile << address << endl;
      existingAddressesFile.close();

      this->waitForRequest();
      return this->getNewEmail();
    } else {
      json jsonResponse = json::parse(this->response);

      this->email = jsonResponse["address"];
      this->id = jsonResponse["id"];

      cout << YELLOW << "--- TEMP MAIL CREATED ---" << endl
           << GREEN << " Email: " << RESET << email << endl
           << GREEN << "󰮔 ID: " << RESET << id << endl;

      this->getToken();
      return true;
    }
  }

  return false;
}

bool TempMail::getToken() {
  this->response.clear();
  curl_easy_setopt(this->curl, CURLOPT_URL, POST_TOKEN.c_str());

  json postData = {{"address", this->email}, {"password", PASSWORD}};

  string jsonBody = postData.dump();

  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1L);

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK &&
      this->response.find("token") != std::string::npos) {
    json jsonResponse = json::parse(this->response);

    this->token = jsonResponse["token"];
    this->setHeaders();

    return true;
  } else {
    this->waitForRequest();
    return getToken();
  }

  return false;
}

string TempMail::generateRandomAddress() {
  string address;
  string alphabet = "abcdefghijklmnopqrstuvwxyz0123456789%#!";
  srand(time(0));

  for (int i = 0; i < this->mailLenght; ++i) {
    int indiceAleatorio = rand() % alphabet.size();
    address += alphabet[indiceAleatorio];
  }

  address = address + getValidDomain();

  if (isExistingAddress(address)) {
    return generateRandomAddress();
  }
  return address;
}

string TempMail::getValidDomain() {

  curl_easy_setopt(this->curl, CURLOPT_URL, GET_DOMAINS.c_str());
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);

  this->code = curl_easy_perform(this->curl);
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 0L);
  this->responseHeaders.clear();

  if (this->code == CURLE_OK &
      this->response.find("domain") != std::string::npos) {
    json jsonResponse = json::parse(this->response);
    string domain = jsonResponse[0]["domain"];
    this->response.clear();

    domain = "@" + domain;
    return domain;
  }

  return "";
}

bool TempMail::isExistingAddress(const std::string &address) {
  ifstream existingAddressesFile(getPath() + EXISTING_ADDRESSES_FILE);
  string line;

  while (getline(existingAddressesFile, line)) {
    if (line == address) {
      return true;
    }
  }

  return false;
}

void TempMail::setHeaders() {

  headers = curl_slist_append(headers, "Host: api.mail.tm");
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Accept-Charset: UTF-8");
  headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.5");
  headers = curl_slist_append(headers, "Referer: https://mail.tm/");
  headers = curl_slist_append(headers,
                              "Content-Type: application/json; charset=UTF-8");
  headers = curl_slist_append(headers, "Origin: https://mail.tm");

  Scrapper::setHeaders();
}

void TempMail::waitForRequest() {
  this_thread::sleep_for(chrono::milliseconds(WAIT_FOR_REQUEST));
}
