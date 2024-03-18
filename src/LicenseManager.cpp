#include "LicenseManager.h"
#include "TempMail/TempMail.h"

LicenseManager::LicenseManager(int numLicenses, int domainLenght) {
  this->numLicenses = numLicenses;
  this->domainLenght = domainLenght;
}

LicenseManager::~LicenseManager() {}

void LicenseManager::generateLicenses() {

  this->generateTempMails();

  for (auto &tempMail : tempMails) {
    Eset eset(tempMail->getEmail());

    eset.CreateAccount();
    this_thread::sleep_for(chrono::milliseconds(100));

    cout << endl << YELLOW << "Waiting to verification code" << endl;

    this->waitForAccountActivation(tempMail, eset);

    int attempts = 0;
    while (!eset.GetLicense() && attempts < 5) {
      cout << YELLOW << "Retrying to get license" << endl;
      attempts++;
    }

    licenses.push_back(eset);
  }

  this->saveGeneratedData(getPath() + "data.csv");
}

void LicenseManager::waitForAccountActivation(unique_ptr<TempMail> &tempMail,
                                              Eset &eset) {
  bool activated = false;
  do {
    tempMail->getMessages();
    for (auto &message : tempMail->messages) {
      if (confirmIfEsetMail(tempMail, eset, message)) {
        activated = true;
        break;
      }
    }
  } while (!activated);
}

bool LicenseManager::confirmIfEsetMail(unique_ptr<TempMail> &tempMail,
                                       Eset &eset, Message &message) {
  if (message.from == "info@product.eset.com" &&
      message.subject == "Account confirmation") {
    tempMail->readMessage(message.id);
    if (eset.ConfirmRegistration(message.body)) {
      return true;
    }
    this_thread::sleep_for(chrono::milliseconds(100));
  }
  return false;
}

void LicenseManager::generateTempMails() {
  for (int i = this->tempMails.size(); i < this->numLicenses; ++i) {
    this->tempMails.emplace_back(make_unique<TempMail>(this->domainLenght));
  }
}

void LicenseManager::saveGeneratedData(const string &filename) {
  if (this->tempMails.size() != this->licenses.size()) {
    cout << RED << "Error: The number of tempMails and licenses is different"
         << RESET << endl;
  }

  ofstream file(filename);

  if (file.is_open()) {
    for (int i = 0; i < this->licenses.size(); i++) {
      file << this->licenses[i].mail << "," << licenses[i].license << endl;
    }

    file.close();
  }
}
