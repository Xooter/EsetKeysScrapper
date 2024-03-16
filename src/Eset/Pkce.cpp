#include "Pkce.h"

Pkce::Pkce() { this->regenerate(); }

Pkce::~Pkce() {}

string Pkce::getAuthorizationUrl() {
  this->regenerate();
  return "/connect/authorize/"
         "callback?client_id=myeset&redirect_uri=https://home.eset.com/"
         "callback&response_type=code&scope=openid mecac "
         "myesetapi&state=" +
         this->state +
         "&code_"
         "challenge=" +
         this->code_challenge +
         "&code_"
         "challenge_method=S256&response_mode=query";
}

void Pkce::regenerate() {
  this->randomString = Crypto::generateRandomString();
  this->state = Crypto::generateRandomString(32, false);
  this->code_verifier = Crypto::base64_url_encode(randomString);
  this->code_challenge = Crypto::sha256_base64url(code_verifier);
}
