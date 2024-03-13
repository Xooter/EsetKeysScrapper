#include "../Proxy.h"

struct Message {
  string id;
  string from;
  string subject;
  string body;
  bool read = false;

  void print() {
    string read = this->read ? "Yes" : "No";
    cout << "\033[1;33mID: \033[0m" << id << endl;
    cout << "\033[1;33mRead: \033[0m" << read << endl;
    cout << "\033[1;33mFrom: \033[0m" << from << endl;
    cout << "\033[1;33mSubject: \033[0m" << subject << endl;
    cout << "\033[1;33mBody: \033[0m" << body << endl;
  }
};
