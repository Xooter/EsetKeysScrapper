#pragma once
#include "../Helpers/Crypto.h"

class Pkce {
public:
  Pkce();
  ~Pkce();

  string getAuthorizationUrl();

  string getState() { return state; }
  string getCodeVerifier() { return code_verifier; }
  string getCodeChallenge() { return code_challenge; }

private:
  void regenerate();

  string randomString;
  string state;
  string code_verifier;
  string code_challenge;
};
