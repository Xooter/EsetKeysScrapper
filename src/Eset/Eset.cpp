#include "Eset.h"
#include <curl/curl.h>

Eset::Eset(string mail) : Scrapper() {

  this->mail = mail;

  const string url = getPath() + "eset_cookies.txt";
  curl_easy_setopt(curl, CURLOPT_COOKIEJAR, url.c_str());
  curl_easy_setopt(curl, CURLOPT_COOKIEFILE, url.c_str());

  this->setHeaders();
}

Eset::~Eset() {}

bool Eset::CreateAccount() {
  this->response.clear();
  this->setHeaders(true);

  json postData = {{"wantReceiveNews", false},
                   {"password", ESET_PASSWORD},
                   {"email", this->mail},
                   {"selectedCountry", "12"}, // must be a string LOL
                   {"agreeWithTerms", true},
                   {"taskId", ""},
                   {"returnUrl", this->pkce.getAuthorizationUrl()},
                   {"browserFingerprint", BROWSER_FINGERPRINT}};

  string jsonBody = postData.dump();

  curl_easy_setopt(this->curl, CURLOPT_URL, CREATE_ACCOUNT.c_str());
  curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
  curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, jsonBody.c_str());

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK && !this->response.empty()) {
    cout << YELLOW << "--- ESSET ACCOUNT CREATED ---" << endl
         << "Email: " << this->mail << endl
         << "Password: " << ESET_PASSWORD << endl;

    this->setHeaders(false);
    return true;
  }

  return false;
}

bool Eset::ConfirmRegistration(string body) {
  this->response.clear();

  string url = getVerificationLink(body);
  setHeaders(false, false);

  curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);

  this->code = curl_easy_perform(this->curl);

  if (this->response.find("We are sorry") != string::npos) {
    cout << RED << "Verification Failed" << endl;
    return false;
  }

  if (this->code == CURLE_OK) {
    cout << GREEN << "Verification Successful" << endl;
    return true;
  }
  return false;
}

string Eset::getVerificationLink(string body) {
  regex linkRegex("\\[([^\\]]+)\\]");

  smatch match;
  if (regex_search(body, match, linkRegex)) {
    if (match.size() > 1) {
      string link = match.str(1);
      return link;
    }
  }
  return "";
}

bool Eset::GetLicense() {
  if (this->token == "") {
    if (this->login())
      cout << "Login" << endl;
    else {
      cout << RED << "Login Failed" << endl;
      return false;
    }
  }
  this_thread::sleep_for(chrono::milliseconds(100));

  if (!activateLicense()) {
    cout << RED << "Error activating license" << endl;
    return false;
  }

  this->response.clear();
  this->responseHeaders.clear();

  this->headers = NULL;

  headers = curl_slist_append(headers, "Host: home.eset.com");
  string tokenHeader = "Authorization: Bearer " + this->token;
  headers = curl_slist_append(headers, tokenHeader.c_str());

  curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);

  curl_easy_setopt(this->curl, CURLOPT_URL, GET_All.c_str());
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);

  this->code = curl_easy_perform(this->curl);

  if (this->response.length() == 2) {
    cout << RED << "No license found" << endl;
    return false;
  }

  if (this->code == CURLE_OK) {
    const json jsonResponse = json::parse(this->response);

    string license = jsonResponse[0]["licenseKey"];

    cout << "License: " << license << endl;
    return true;
  }

  return false;
}

bool Eset::activateLicense() {
  this->response.clear();

  struct curl_slist *headers = NULL;

  headers = curl_slist_append(headers, "Host: home.eset.com");
  headers = curl_slist_append(headers, "Content-type: application/json");
  headers = curl_slist_append(
      headers, "Referer: https://home.eset.com/protect/installer");
  string tokenHeader = "Authorization: Bearer " + this->token;
  headers = curl_slist_append(headers, tokenHeader.c_str());

  curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);

  curl_easy_setopt(this->curl, CURLOPT_URL, ACTIVATE_lICENSE.c_str());
  curl_easy_setopt(this->curl, CURLOPT_POST, 1L);

  const string activateData =
      "{\"operatingSystem\":9,\"sendEmail\":false,\"emailAddress\":\"" +
      this->mail + "\",\"freemiumType\":1}";

  curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, activateData.c_str());

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK && this->response.find("!DOCTYPE") == string::npos)
    return true;

  return false;
}

bool Eset::login() {
  this->response.clear();

  const string hardcodedJson =
      "{\"email\":\"" + this->mail + "\",\"password\":\"" + ESET_PASSWORD +
      "\",\"returnUrl\":\"" + this->pkce.getAuthorizationUrl() +
      "\",\"browserFingerprint\":\"" + BROWSER_FINGERPRINT + "\"}";

  curl_easy_setopt(this->curl, CURLOPT_URL, LOGIN_ACCOUNT.c_str());
  curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
  curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, hardcodedJson.c_str());

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK &&
      this->response.find("redirectUrl") != string::npos) {
    const json jsonResponse = json::parse(this->response);

    if (jsonResponse["result"] == 1) {
      cout << RED << "Account does not exist" << endl;
      return false;
    } else if (this->response.find("\"https://home.eset.com\"") !=
               string::npos) {
      cout << RED << "Error in PKCE callback" << endl;
      return false;
    }

    string authLocationHeader = getPKCEResponse(this->response);

    const string authLocation = getAuthLocation(this->responseHeaders);
    const string code = getParameterValue(authLocation, "code");

    if (!getAccessToken(code)) {
      cout << RED << "Error getting JWT token" << endl;
      return false;
    }

    return true;
  }
  return false;
}

string Eset::getPKCEResponse(string response) {
  this->response.clear();

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(
      headers,
      "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/"
      "avif,image/webp,image/apng,*/*;q=0.8,application/"
      "signed-exchange;v=b3;q=0.7");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  const json jsonResponse = json::parse(response);

  string redirectUrl = jsonResponse["redirectUrl"];
  redirectUrl =
      "https://login.eset.com" + regex_replace(redirectUrl, regex(" "), "%20");

  curl_easy_setopt(this->curl, CURLOPT_URL, redirectUrl.c_str());
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);

  this->code = curl_easy_perform(this->curl);

  setHeaders();

  return this->responseHeaders;
}

bool Eset::getAccessToken(string code) {
  this->response.clear();
  this->responseHeaders.clear();

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(
      headers, "Content-Type: application/x-www-form-urlencoded");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  const string tokenBody =
      "client_id=myeset&code=" + code +
      "&redirect_uri=https%3A%2F%2Fhome.eset.com%2Fcallback&code_verifier=" +
      pkce.getCodeVerifier() + "&grant_type=authorization_code";

  curl_easy_setopt(this->curl, CURLOPT_URL, TOKEN.c_str());
  curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
  curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, tokenBody.c_str());

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK) {
    const json jsonResponse = json::parse(this->response);

    string access_token = jsonResponse["access_token"];

    this->token = access_token;
    setHeaders();
    return true;
  }

  return false;
}

string Eset::getAuthLocation(string header) {
  std::size_t locationIndex = header.find("location: ");
  if (locationIndex != std::string::npos) {
    std::string locationSubstring = header.substr(locationIndex + 10);
    return locationSubstring;
  }
  return "";
}

string Eset::getParameterValue(const std::string &url,
                               const std::string &parameter) {
  std::regex pattern(parameter + "=([^&]+)");
  std::smatch match;

  if (std::regex_search(url, match, pattern)) {
    return match[1];
  }

  return "";
}

void Eset::setHeaders(bool create_account, bool hasLenght) {
  headers = NULL;
  headers = curl_slist_append(headers, "Host: login.eset.com");

  if (create_account)
    headers = curl_slist_append(
        headers, "Referer: https://login.eset.com/register/final-step");
  else {
    headers =
        curl_slist_append(headers, "Referer: https://login.eset.com/login");
    if (hasLenght)
      headers = curl_slist_append(headers, "Content-Length: 416"); // 420!!!
  }

  headers =
      curl_slist_append(headers, "Accept: application/json, text/plain, */*");
  headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.5");
  headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br");
  headers = curl_slist_append(headers, "X-Security-Request: required");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "Origin: https://login.eset.com");
  headers = curl_slist_append(headers, "Connection: keep-alive");
  headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");
  headers = curl_slist_append(
      headers, "Sec-Ch-Ua: 'Chromium';v='121', 'Not A(Brand';v='99'");
  headers = curl_slist_append(headers, "Cache-Control: no-cache");
  headers = curl_slist_append(headers, "TE: trailers");
  Scrapper::setHeaders();
}
