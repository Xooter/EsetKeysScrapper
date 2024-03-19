#pragma once

#include "Eset/Eset.h"
#include "ProxyReader.h"
#include "TempMail/TempMail.h"

class LicenseManager {
public:
  LicenseManager(int numLicenses, int domainLenght, string proxyFile);
  ~LicenseManager();

  void generateLicenses();

  void showAllLicenses();

private:
  int numLicenses = 1;
  int domainLenght = 15;

  ProxyReader proxyReader;
  bool useProxies = false;

  vector<unique_ptr<TempMail>> tempMails;
  vector<Eset> licenses;

  void generateTempMails();
  void waitForAccountActivation(unique_ptr<TempMail> &tempMail, Eset &eset);
  bool confirmIfEsetMail(unique_ptr<TempMail> &tempMail, Eset &eset,
                         Message &message);

  void saveGeneratedData(const string &filename);
};
