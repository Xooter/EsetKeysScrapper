#pragma once
#include "../Helpers/Crypto.h"

class Pkce {
public:
  Pkce();
  ~Pkce();

  string getCodeVerifier() { return code_verifier; }
  string getCodeChallenge() { return code_challenge; }
  string getState() { return state; }

private:
  string randomString;
  string state;
  string code_verifier;
  string code_challenge;
};
