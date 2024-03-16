#include "src/Eset/Eset.h"
#include "src/TempMail/TempMail.h"
#include <chrono>

using namespace std;

vector<unique_ptr<TempMail>> readTempMailsFromFile(const string &filename) {
  vector<unique_ptr<TempMail>> tempMails;
  ifstream file(filename);

  cout << "Searching cached mails in file: " << endl << filename << endl;

  if (file.is_open()) {
    string line;

    while (getline(file, line)) {
      size_t pos = line.find(":");
      if (pos != std::string::npos) {
        string id = line.substr(0, pos);
        string email = line.substr(pos + 1);

        cout << "---" << endl
             << "\033[1;31mEmail: \033[0m" << email << endl
             << "\033[1;31mID: \033[0m" << id << endl;

        tempMails.emplace_back(make_unique<TempMail>(id, email));
      }
    }
    file.close();
  }

  return tempMails;
}

void writeTempMailsToFile(const string &filename,
                          vector<unique_ptr<TempMail>> &tempMails) {
  ofstream file(filename);

  if (file.is_open()) {
    for (auto &tempMail : tempMails) {
      file << tempMail->getId() << ":" << tempMail->getEmail() << endl;
    }

    file.close();
  }
}

int main() {
  vector<unique_ptr<TempMail>> existingTempMails =
      readTempMailsFromFile(getPath() + "mails.txt");

  int numNewTempMails = 1;
  if (existingTempMails.size() < numNewTempMails) {
    cout << "Creating new temp mails..." << endl;
  }

  for (int i = existingTempMails.size(); i < numNewTempMails; ++i) {
    existingTempMails.emplace_back(make_unique<TempMail>());
  }

  writeTempMailsToFile(getPath() + "mails.txt", existingTempMails);

  Eset eset(existingTempMails[0]->getEmail());

  eset.CreateAccount();
  this_thread::sleep_for(chrono::milliseconds(100));

  bool activated = false;

  while (!activated) {
    for (auto &tempMail : existingTempMails) {
      cout << YELLOW << tempMail->getEmail() << endl;
      tempMail->getMessages();
      cout << "Messages: " << tempMail->messages.size() << endl;
      for (auto &message : tempMail->messages) {
        if (message.from == "info@product.eset.com" &&
            message.subject == "Account confirmation") {
          tempMail->readMessage(message.id);
          if (eset.ConfirmRegistration(message.body)) {
            activated = true;
            break;
          }
          this_thread::sleep_for(chrono::milliseconds(200));
        }
      }
    }
  }

  eset.GetLicense();

  return 0;
}
