#pragma once
#include "Message.h"
using namespace std;

inline const string API_URL = "https://api.mail.tm";
inline const string CREATE_MAIL = API_URL + "/accounts";
inline const string DELETE_MAIL = (API_URL + "/accounts/{}");
inline const string GET_ME = (API_URL + "/me");

inline const string GET_DOMAINS = (API_URL + "/domains");
inline const string GET_DOMAIN = (API_URL + "/domains/{}");

inline const string GET_MESSAGES =
    (API_URL + "/messages?page=1"); // cambiar page por la pagina, se banca
                                    // hasta 30 mensajesa
inline const string GET_MESSAGE = (API_URL + "/messages/");

inline const string POST_TOKEN = (API_URL + "/token");

inline const int WAIT_FOR_REQUEST = 500; // milliseconds

inline const string EXISTING_ADDRESSES_FILE = "existing_addresses.txt";
inline const string PASSWORD = "MarketInfluencer";

class TempMail {
public:
  vector<Message> messages;

  TempMail(string id = "", string address = "");
  ~TempMail();

  bool getNewEmail();
  bool getMessages();
  Message readMessage(string id);

  string getEmail() { return email; }
  string getId() { return id; }

private:
  CURL *curl;
  CURLcode code;

  string email;
  string id;
  string password = "MarketInfluencer";

  string response;
  string token;

  bool getToken();
  bool isExistingAddress(const std::string &address);

  vector<std::string> userAgents{
      "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, "
      "like Gecko) Chrome/74.0.3729.169 Safari/537.36",
      "Mozilla/5.0 (iPhone; CPU iPhone OS 12_2 like Mac OS X) "
      "AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148",
      "Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"};

  string generateRandomAddress();

  void waitForRequest();
  string pickUserAgent();
  void setHeaders();

  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              void *userp);
  struct curl_slist *headers = NULL;
};
