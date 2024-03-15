#include "Eset.h"
#include <curl/curl.h>

Eset::Eset() : Scrapper() {
  // temporal
  const string url = getPath() + "cookies.txt";
  curl_easy_setopt(curl, CURLOPT_COOKIEJAR, url.c_str());
  curl_easy_setopt(curl, CURLOPT_COOKIEFILE, url.c_str());
  //

  this->setHeaders();
}

Eset::~Eset() {}

bool Eset::CreateAccount(string email) {
  this->response.clear();

  json postData = {
      {"wantReceiveNews", false},
      {"password", ESET_PASSWORD},
      {"email", email},
      {"selectedCountry", "12"}, // must be a string LOL
      {"agreeWithTerms", true},
      {"taskId", ""},
      {"returnUrl",
       "/connect/authorize/callback?client_id=myeset&redirect_uri=https,//"
       "home.eset.com/"
       "callback&response_type=code&scope=openidmecacmyesetapi&state="
       "ea369ac85061471d99a492165cf052cb&code_challenge=7_a638flArcIjbqlF_"
       "dj3IQ5S9pfRI80GZrBeB_GIbM&code_challenge_method=S256&response_"
       "mode=query"},
      {"browserFingerprint", "593b2e9b81d07f79a98bb130b99bdc80"}};

  string jsonBody = postData.dump();

  cout << jsonBody << endl;

  curl_easy_setopt(this->curl, CURLOPT_URL, CREATE_ACCOUNT.c_str());
  curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonBody.c_str());

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK && !this->response.empty()) {
    cout << "--- Cuenta Eset Creada ---" << endl
         << "Email: " << email << endl
         << "password" << ESET_PASSWORD << endl;

    return true;
  }

  return false;
}

bool Eset::ConfirmRegistration(string body) {
  this->response.clear();

  string url = getVerificationLink(body);

  curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(this->curl, CURLOPT_FOLLOWLOCATION, 1L);

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK) {
    cout << endl << "Verification Successful" << endl;
    return true;
  }
  return false;
}

string Eset::getVerificationLink(string body) {
  std::regex linkRegex("\\[([^\\]]+)\\]");

  std::smatch match;
  if (std::regex_search(body, match, linkRegex)) {
    if (match.size() > 1) {
      std::string link = match.str(1);
      std::cout << endl << "Enlace Confirm registration: " << link << std::endl;
      return link;
    }
  }
  return "";
}

bool Eset::GetLicense() {
  if (this->token == "")
    this->login();

  activateLicense();

  this->response.clear();

  curl_easy_setopt(this->curl, CURLOPT_URL, GET_All.c_str());
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);

  this->code = curl_easy_perform(this->curl);

  const json jsonResponse = json::parse(this->response);

  string license = jsonResponse[0]["licenseKey"];

  cout << "License: " << license << endl;

  return true;
}

bool Eset::activateLicense() {
  this->response.clear();

  struct curl_slist *headers = NULL;

  headers = curl_slist_append(headers, "Host: home.eset.com");

  if (this->token != "") {
    string tokenHeader = "Authorization: Bearer " + this->token;
    headers = curl_slist_append(headers, tokenHeader.c_str());
  }
  curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);

  curl_easy_setopt(this->curl, CURLOPT_URL,
                   "https://home.eset.com/api/License/ProtectDevice");
  curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
  curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS,
                   "{\"operatingSystem\":9,\"sendEmail\":true,\"emailAddress\":"
                   "\"qke9m1jf4k5w#@yogirt.com\",\"freemiumType\":1}");

  this->code = curl_easy_perform(this->curl);

  return true;
}

bool Eset::login() {
  this->response.clear();

  const string randomString = Crypto::generateRandomString();
  const string state = Crypto::generateRandomString(32, false);
  const string code_verifier = Crypto::base64_url_encode(randomString);
  const string code_challenge = Crypto::sha256_base64url(code_verifier);

  const string hardcodedJson =
      "{\"email\":\"qke9m1jf4k5w#@yogirt.com\",\"password\":"
      "\"" +
      ESET_PASSWORD +
      "\",\"returnUrl\":\"/connect/authorize/"
      "callback?client_id=myeset&redirect_uri=https://home.eset.com/"
      "callback&response_type=code&scope=openid mecac "
      "myesetapi&state=" +
      state +
      "&code_"
      "challenge=" +
      code_challenge +
      "&code_"
      "challenge_method=S256&response_mode=query\","
      "\"browserFingerprint\":\"cb8dbe7a3b8f850232bbcf15f8c6e004\"}";

  curl_easy_setopt(this->curl, CURLOPT_URL, LOGIN_ACCOUNT.c_str());
  curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
  curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, hardcodedJson.c_str());

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK &&
      this->response.find("redirectUrl") != string::npos) {

    string authLocationHeader = getPKCEResponse(this->response);

    const string authLocation = getAuthLocation(this->responseHeaders);
    const string code = getParameterValue(authLocation, "code");

    getAccessToken(code, code_verifier);

    return true;
  }
  return false;
}

string Eset::getPKCEResponse(string response) {
  this->response.clear();

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(
      headers, "Accept: "
               "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
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

  return this->responseHeaders;
}

bool Eset::getAccessToken(string code, string code_verifier) {
  this->response.clear();

  const string tokenBody =
      "client_id=myeset&code=" + code +
      "&redirect_uri=https%3A%2F%2Fhome.eset.com%2Fcallback&code_verifier=" +
      code_verifier + "&grant_type=authorization_code";

  curl_easy_setopt(this->curl, CURLOPT_URL, TOKEN.c_str());
  curl_easy_setopt(this->curl, CURLOPT_POST, 1L);
  curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, tokenBody.c_str());

  this->code = curl_easy_perform(this->curl);

  if (this->code == CURLE_OK) {
    const json jsonResponse = json::parse(this->response);

    string access_token = jsonResponse["access_token"];

    this->token = access_token;
    setHeaders();
  }

  return true;
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

void Eset::setHeaders() {

  headers = curl_slist_append(headers, "Host: login.eset.com");
  headers = curl_slist_append(headers, "Referer: https://login.eset.com/login");
  headers =
      curl_slist_append(headers, "Accept: application/json, text/plain, */*");
  headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.5");
  headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br");
  headers = curl_slist_append(headers, "X-Security-Request: required");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "Origin: https://login.eset.com");
  headers = curl_slist_append(headers, "Connection: keep-alive");
  headers = curl_slist_append(headers, "Content-Length: 416");
  headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");
  headers = curl_slist_append(
      headers, "Sec-Ch-Ua: 'Chromium';v='121', 'Not A(Brand';v='99'");
  /* headers = curl_slist_append( */
  /*     headers, "Referer: https://login.eset.com/register/final-step"); */
  headers = curl_slist_append(headers, "Cache-Control: no-cache");
  headers = curl_slist_append(headers, "TE: trailers");
  Scrapper::setHeaders();
}
