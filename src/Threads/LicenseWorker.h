#ifndef LICENSEWORKER_H
#define LICENSEWORKER_H
#include "../LicenseManager.h"
#include <QObject>
#include <QString>
#include <string>

class LicenseWorker : public QObject {
  Q_OBJECT
public:
  int numLicenses;
  int domainLenght;
  std::string proxyFile;

  LicenseWorker(int n, int d, std::string p)
      : numLicenses(n), domainLenght(d), proxyFile(p) {}

signals:
  void finished(const QString &license);

public slots:
  void process() {
    LicenseManager manager(numLicenses, domainLenght, proxyFile);
    manager.generateLicenses();

    QString firstLicense;
    auto licenses = manager.getLicenses();
    if (!licenses.empty())
      firstLicense = QString::fromStdString(licenses[0].license);

    emit finished(firstLicense);
  }
};
#endif
