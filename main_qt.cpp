#include "src/LicenseManager.h"
#include "src/Threads/LicenseWorker.h"
#include <QApplication>
#include <QClipboard>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QWidget window;
  window.setWindowTitle("EsetKeys GUI");

  QVBoxLayout *layout = new QVBoxLayout(&window);

  QTextEdit *result = new QTextEdit();
  result->setReadOnly(true);
  result->setMaximumHeight(80);

  QPushButton *copyBtn = new QPushButton("Copy");
  QPushButton *generateBtn = new QPushButton("Generate");

  layout->addWidget(result);
  layout->addWidget(copyBtn);
  layout->addWidget(generateBtn);

  window.setLayout(layout);
  window.resize(400, 200);
  window.show();

  auto generateLicenses = [&]() {
    result->setText("Generating...");
    QApplication::processEvents();

    LicenseManager manager(1, 10, "");
    manager.generateLicenses();

    QString firstLicense;
    auto licenses = manager.getLicenses();
    if (!licenses.empty())
      firstLicense = QString::fromStdString(licenses[0].license);

    result->setText(firstLicense);
  };

  QObject::connect(copyBtn, &QPushButton::clicked, [=]() {
    QApplication::clipboard()->setText(result->toPlainText());
  });

  QObject::connect(generateBtn, &QPushButton::clicked, generateLicenses);
  generateLicenses();

  return app.exec();
}

#include "main_qt.moc"
