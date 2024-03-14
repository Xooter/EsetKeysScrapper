#pragma once

#include "../Scrapper.h"

inline const string ESET_PASSWORD = "rula123123123R";

inline const string GET_All =
    "https://home.eset.com/api/License/GetAll"; // Necesita Token jwt y cookie
                                                // Cookie Cookie
                                                // TS011f1e89=016c9a7a13a90a356651e609640dc221c5d2931a5d12cdbc1750d5e136c266cf38f2dfec3f9b668acc37474b15bcc4309fc57d63c2;
                                                // ARRAffinity=8e5613aa7caf6778ed6e2eb814a21d1a52332a91598512e62a9a888003931420;
                                                // ARRAffinitySameSite=8e5613aa7caf6778ed6e2eb814a21d1a52332a91598512e62a9a888003931420;
                                                // TS0165daf9=016c9a7a13de18fdf999cba64fe6c7d94402222377f9c7c5c88112fefb1ecb813dfc613ac0d4cb9a3fa01f85a66ad31f742fd22133

inline const string CREATE_ACCOUNT = "https://login.eset.com/api/Account/"
                                     "Create";

class Eset : public Scrapper {
public:
  Eset();
  ~Eset();

  bool CreateAccount(string email);
  bool ConfirmRegistration(string body);

private:
  void setHeaders();
};
