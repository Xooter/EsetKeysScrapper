#include "src/Eset/Eset.h"
#include "src/TempMail/TempMail.h"
#include <chrono>
#include <thread>

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

  int numNewTempMails = 2;
  if (existingTempMails.size() < numNewTempMails) {
    cout << "Creating new temp mails" << endl;
  }

  for (int i = existingTempMails.size(); i < numNewTempMails; ++i) {
    existingTempMails.emplace_back(make_unique<TempMail>());
  }

  writeTempMailsToFile(getPath() + "mails.txt", existingTempMails);

  Eset eset;

  eset.CreateAccount(existingTempMails[0]->getEmail());

  this_thread::sleep_for(chrono::milliseconds(5000));

  for (auto &tempMail : existingTempMails) {
    cout << "\033[1;33m--- " << tempMail->getEmail() << " ---\033[0m" << endl;
    tempMail->getMessages();
    cout << "Messages: " << tempMail->messages.size() << endl;
    for (auto &message : tempMail->messages) {
      cout << "\033[1;31m--- Message ---\033[0m" << endl;
      tempMail->readMessage(message.id);
      message.print();
      eset.ConfirmRegistration(message.body);
    }
  }
  eset.GetLicense();

  return 0;
}
