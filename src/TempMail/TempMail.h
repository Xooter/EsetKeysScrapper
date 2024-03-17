#pragma once
#include "Message.h"

inline const string API_URL = "https://api.mail.tm";
inline const string CREATE_MAIL = API_URL + "/accounts";
inline const string DELETE_MAIL = (API_URL + "/accounts/{}");
inline const string GET_ME = (API_URL + "/me");

inline const string GET_DOMAINS = (API_URL + "/domains");
inline const string GET_DOMAIN = (API_URL + "/domains/{}");

inline const string GET_MESSAGES =
    (API_URL +
     "/messages?page=1"); // cambiar page por la pagina, 30 msg per page
inline const string GET_MESSAGE = (API_URL + "/messages/");

inline const string POST_TOKEN = (API_URL + "/token");

inline const int WAIT_FOR_REQUEST = 500; // milliseconds

inline const string EXISTING_ADDRESSES_FILE = "existing_addresses.txt";
inline const string PASSWORD = "EsetKeys";

class TempMail : public Scrapper {
public:
  vector<Message> messages;

  TempMail(int mailLenght);
  ~TempMail();

  bool getNewEmail();
  bool getMessages();
  Message readMessage(string id);

  string getEmail() { return email; }
  string getId() { return id; }

private:
  string email;
  string id;
  string password = PASSWORD;
  int mailLenght = 15;

  void setHeaders() override;

  bool getToken();
  bool isExistingAddress(const std::string &address);

  string generateRandomAddress();

  void waitForRequest();
};
