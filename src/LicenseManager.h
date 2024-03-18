#pragma once

#include "Eset/Eset.h"
#include "TempMail/TempMail.h"

class LicenseManager {
public:
  LicenseManager(int numLicenses, int domainLenght);
  ~LicenseManager();

  void generateLicenses();

private:
  int numLicenses = 1;
  int domainLenght = 15;

  vector<unique_ptr<TempMail>> tempMails;
  vector<Eset> licenses;

  void generateTempMails();
  void waitForAccountActivation(unique_ptr<TempMail> &tempMail, Eset &eset);
  bool confirmIfEsetMail(unique_ptr<TempMail> &tempMail, Eset &eset,
                         Message &message);

  void saveGeneratedData(const string &filename);
};
