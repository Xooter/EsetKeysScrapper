#include "src/TempMail/TempMail.h"

using namespace std;

vector<TempMail> readTempMailsFromFile(const string &filename) {
  vector<TempMail> tempMails;
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
             << "\033[1;31mID: \033[0m" << id << endl
             << "---" << endl;

        tempMails.emplace_back(id, email);
      }
    }
    file.close();
  }

  return tempMails;
}

void writeTempMailsToFile(const string &filename, vector<TempMail> &tempMails) {
  ofstream file(filename);

  if (file.is_open()) {
    for (auto &tempMail : tempMails) {
      file << tempMail.getId() << ":" << tempMail.getEmail() << endl;
    }

    file.close();
  }
}

int main() {
  vector<TempMail> existingTempMails =
      readTempMailsFromFile(getPath() + "mails.txt");

  int numNewTempMails = 1;
  if (existingTempMails.size() < numNewTempMails) {
    cout << "Creating new temp mails" << endl;
  }

  for (int i = existingTempMails.size(); i < numNewTempMails; ++i) {
    existingTempMails.emplace_back();
  }

  for (auto &tempMail : existingTempMails) {
    tempMail.getMessages();
    for (auto &message : tempMail.messages) {
      cout << "\033[1;31m--- Message ---\033[0m" << endl;
      tempMail.readMessage(message.id);
      message.print();
    }
  }

  writeTempMailsToFile(getPath() + "mails.txt", existingTempMails);
  return 0;
}
