#include "Eset.h"

Eset::Eset() : Scrapper() { this->setHeaders(); }

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

    return true;
  }

  return false;
}

bool Eset::ConfirmRegistration(string body) {
  this->response.clear();

  string url;
  std::regex linkRegex("\\[([^\\]]+)\\]");

  std::smatch match;
  if (std::regex_search(body, match, linkRegex)) {
    if (match.size() > 1) {
      std::string link = match.str(1);
      std::cout << endl << "Enlace Confirm registration: " << link << std::endl;
    }
  }
  curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(this->curl, CURLOPT_HTTPGET, 1L);

  this->code = curl_easy_perform(this->curl);
  cout << this->response << this->code << endl;
  /**/
  /* if (this->code == CURLE_OK) { */
  /*   return true; */
  /* } */
  return false;
}

void Eset::setHeaders() {

  headers = curl_slist_append(headers, "Host: login.eset.com");
  headers =
      curl_slist_append(headers, "Accept: application/json, text/plain, */*");
  headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.5");
  headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br");
  headers = curl_slist_append(headers, "X-Security-Request: required");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "Origin: https://login.eset.com");
  headers = curl_slist_append(headers, "Connection: keep-alive");
  headers = curl_slist_append(
      headers, "Referer: https://login.eset.com/register/final-step");
  /* headers = curl_slist_append( */
  /*     headers, */
  /*     "Cookie: " */
  /*     "TS0165daf9=" */
  /*     "016c9a7a1316bb6c95c68839c681f4856d8e1b5eb448b37fa09c6055b3a825ece1c9aa99"
   */
  /*     "e08adc82ec9de399218f7d958b1af5b200; " */
  /*     "_ga_QQY56YDFHV=GS1.1.1710378917.1.0.1710378920.0.0.0; " */
  /*     "_ga=GA1.1.322127407.1710378917; " */
  /*     "TS011aa10a=" */
  /*     "016c9a7a13d7fff668adab4922c6962fc555886ff02a815c1b9137be55ffb2e49eb66133"
   */
  /*     "ff2c6d3a84499a974ade11f5708b41501a; " */
  /*     "ARRAffinity=" */
  /*     "994d6037a594b0791ca005e2c92d0e014e04aca1e170bc15a30c7bda75752344; " */
  /*     "ARRAffinitySameSite=" */
  /*     "994d6037a594b0791ca005e2c92d0e014e04aca1e170bc15a30c7bda75752344; " */
  /*     "TS0129100e=016c9a7a13e10…562beddbede; TS0191ffa1=" */
  /*     "0142213e1aa239b131cbd3c600032137820f48d22916a9bfa3de33dca4593b0b2a9fddb6"
   */
  /*     "8ccb5a4bcc4bc89a9e828c2de7f9c6133d; " */
  /*     "TS0129100e030=" */
  /*     "01d6f3b750331278725ecc332a6684e686cb7d3d68a8820f83d1efc5945717dfab3bbe47"
   */
  /*     "58b6368446ceb06ffaa005416720c631a0; " */
  /*     "TS8ed395af029=" */
  /*     "086606e5dfab28000765b4f2bb1184f5e9f39589f0bcbe20c05141b235589593ded7f75f"
   */
  /*     "bc3559c5591726571388965d"); */
  headers = curl_slist_append(headers, "Sec-Fetch-Dest: empty");
  headers = curl_slist_append(headers, "Sec-Fetch-Mode: cors");
  headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
  headers = curl_slist_append(headers, "Pragma: no-cache");
  headers = curl_slist_append(headers, "Cache-Control: no-cache");
  Scrapper::setHeaders();
}
