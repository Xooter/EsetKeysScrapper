#pragma once

#include "../Scrapper.h"
#include "Pkce.h"

inline const string ESET_PASSWORD = "rula123123123R";
inline const string BROWSER_FINGERPRINT = "593b2e9b81d07f79a98bb130b99bdc80";

inline const string CREATE_ACCOUNT =
    "https://login.eset.com/api/Account/Create";

inline const string LOGIN_ACCOUNT = "https://login.eset.com/api/Account/Login";
inline const string TOKEN = "https://login.eset.com/connect/token";
inline const string ACTIVATE_lICENSE =
    "https://home.eset.com/api/License/ProtectDevice";
inline const string GET_All = "https://home.eset.com/api/License/GetAll";

class Eset : public Scrapper {
public:
  Eset(string mail);
  ~Eset();

  bool CreateAccount();
  bool ConfirmRegistration(string body);
  bool GetLicense();

  string license = "";

private:
  Pkce pkce;

  string mail;

  string getVerificationLink(string body);
  void setHeaders(bool create_account = false, bool hasLenght = true);

  string getAuthLocation(string header);
  string getParameterValue(const std::string &url,
                           const std::string &parameter);

  bool getAccessToken(string code);
  string getPKCEResponse(string response);
  bool login();
  bool activateLicense();
};
