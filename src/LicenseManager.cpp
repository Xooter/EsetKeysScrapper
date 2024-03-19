#include "LicenseManager.h"

LicenseManager::LicenseManager(int numLicenses, int domainLenght,
                               string proxyFile) {
  this->numLicenses = numLicenses;
  this->domainLenght = domainLenght;
  if (!proxyFile.empty()) {
    cout << GREEN << "---" << RESET << endl;
    cout << GREEN << "Loading proxies file..." << RESET << endl;
    cout << YELLOW << proxyFile << RESET << endl;
    cout << GREEN << "---" << RESET << endl << endl;
    this->proxyReader.readProxies(proxyFile);
    this->useProxies = true;
  }
}

LicenseManager::~LicenseManager() {}

void LicenseManager::generateLicenses() {

  this->generateTempMails();

  if (useProxies) {

    cout << endl << YELLOW << "--- Using proxies ---" << endl;
    cout << " This can take a while depending on the latency of the proxy"
         << endl
         << "--- ---" << endl;
  }

  for (auto &tempMail : tempMails) {

    Proxy proxy("http://127.0.0.1:80");
    if (useProxies) {
      do {
        proxy = this->proxyReader.giveNext();
      } while (proxy.isWorking());
    }

    Eset eset(tempMail->getEmail(), useProxies ? &proxy : nullptr);

    int attempts_creating_account = 0;
    while (!eset.CreateAccount() && attempts_creating_account < 5) {
      cout << YELLOW << "󰑓 Retrying to create account..." << RESET << endl;
      attempts_creating_account++;
    }

    if (attempts_creating_account > 5) {
      cout << RED << " Error creating account" << RESET << endl;
      continue;
    }

    this_thread::sleep_for(chrono::milliseconds(100));

    cout << endl << YELLOW << " Waiting to verification code..." << endl;

    this->waitForAccountActivation(tempMail, eset);

    int attempts = 0;
    while (!eset.GetLicense() && attempts < 5) {
      cout << YELLOW << "󰑓 Retrying to get license..." << endl;
      attempts++;
    }

    if (attempts > 5) {
      cout << RED << " Error getting license" << RESET << endl;
      continue;
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
  ofstream file(filename);

  if (file.is_open()) {
    for (int i = 0; i < this->licenses.size(); i++) {
      if (licenses[i].license != "" || licenses[i].mail != "")
        continue;
      file << this->licenses[i].mail << "," << licenses[i].license << endl;
    }

    file.close();
  }
}

void LicenseManager::showAllLicenses() {
  for (auto &license : licenses) {
    cout << GREEN << " Mail: " << RESET << license.mail << endl;
    cout << GREEN << "󰌆 License: " << RESET << license.license << endl;
    cout << endl;
  }
}
