#include "Pkce.h"

Pkce::Pkce() {
  this->randomString = Crypto::generateRandomString();
  this->state = Crypto::generateRandomString(32, false);
  this->code_verifier = Crypto::base64_url_encode(randomString);
  this->code_challenge = Crypto::sha256_base64url(code_verifier);
}

Pkce::~Pkce() {}
