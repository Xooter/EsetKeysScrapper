#pragma once

#include "../Scrapper.h"
#include "Pkce.h"

inline const string ESET_PASSWORD = "rula123123123R";

inline const string CREATE_ACCOUNT = "https://login.eset.com/api/Account/"
                                     "Create";

inline const string LOGIN_ACCOUNT = "https://login.eset.com/api/Account/Login";
inline const string TOKEN = "https://login.eset.com/connect/token";
inline const string GET_All = "https://home.eset.com/api/License/GetAll";

class Eset : public Scrapper {
public:
  Eset();
  ~Eset();

  bool CreateAccount(string email);
  bool ConfirmRegistration(string body);
  bool GetLicense();

private:
  Pkce pkce;

  string getVerificationLink(string body);
  void setHeaders();

  string getAuthLocation(string header);
  string getParameterValue(const std::string &url,
                           const std::string &parameter);

  bool getAccessToken(string code);
  string getPKCEResponse(string response);
  bool login();
  bool activateLicense();
};
