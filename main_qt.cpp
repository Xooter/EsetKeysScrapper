#include "src/LicenseManager.h"
#include <QApplication>
#include <QClipboard>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setStyle("fusion");

  QWidget window;
  window.setWindowTitle("EsetKeysScrapper");
  window.setFixedSize(520, 400);
  window.setAttribute(Qt::WA_TranslucentBackground);

  app.setStyleSheet(R"(
      QWidget {
        color: #f1f1f1;
        font-family: 'Segoe UI';
        font-size: 14px;
      }

      QFrame#mainCard {
        background-color: #2b2d31;
        border-radius: 14px;
      }

      QLabel#title {
        font-size: 22px;
        font-weight: 700;
        color: #ffffff;
        margin-bottom: 8px;
      }

      QLabel#disclaimerTitle {
        font-size: 13px;
        font-weight: bold;
        letter-spacing: 2px;
        color: #6ab04c;
        margin-top: 4px;
      }

      QLabel#disclaimer {
        font-size: 12px;
        color: #aaaaaa;
        line-height: 1.4em;
        margin-bottom: 12px;
      }

      QTextEdit {
        border: 1px solid #3d3f44;
        border-radius: 10px;
        background-color: #1b1c1e;
        padding: 14px;
        font-family: Consolas, monospace;
        font-size: 16px;
        font-weight: 600;
        color: #00ff9c;
        text-align: center;
      }

      QPushButton {
        border: none;
        border-radius: 8px;
        padding: 10px 20px;
        font-weight: 600;
        background-color: #0078d7;
        color: #ffffff;
        transition: all 0.2s ease-in-out;
      }

      QPushButton:hover {
        background-color: #3794ff;
      }

      QPushButton:disabled {
        background-color: #55585c;
        color: #aaaaaa;
      }
  )");

  QFrame *card = new QFrame();
  card->setObjectName("mainCard");

  QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
  shadow->setBlurRadius(25);
  shadow->setOffset(0, 6);
  shadow->setColor(QColor(0, 0, 0, 160));
  card->setGraphicsEffect(shadow);

  QLabel *title = new QLabel("🔑 EsetKeysScrapper");
  title->setObjectName("title");
  title->setAlignment(Qt::AlignCenter);

  QLabel *disclaimerTitle = new QLabel("D I S C L A I M E R");
  disclaimerTitle->setObjectName("disclaimerTitle");
  disclaimerTitle->setAlignment(Qt::AlignCenter);

  QLabel *disclaimerText = new QLabel(
      "EsetKeysScrapper is intended for educational and research purposes only.<br>"
      "The developers assume no responsibility for any misuse or illegal activities conducted with this tool.<br>"
      "Users are advised to comply with all relevant laws and ethical guidelines when using this software."
  );
  disclaimerText->setObjectName("disclaimer");
  disclaimerText->setAlignment(Qt::AlignCenter);
  disclaimerText->setWordWrap(true);

  QTextEdit *result = new QTextEdit();
  result->setReadOnly(true);
  result->setFixedHeight(100);
  result->setAlignment(Qt::AlignCenter);

  QPushButton *copyBtn = new QPushButton("📋 Copy");
  QPushButton *generateBtn = new QPushButton("⚙️ Generate");

  QHBoxLayout *buttons = new QHBoxLayout();
  buttons->addStretch();
  buttons->addWidget(copyBtn);
  buttons->addWidget(generateBtn);
  buttons->addStretch();

  QVBoxLayout *layout = new QVBoxLayout(card);
  layout->setContentsMargins(24, 24, 24, 24);
  layout->setSpacing(16);
  layout->addWidget(title);
  layout->addWidget(disclaimerTitle);
  layout->addWidget(disclaimerText);
  layout->addSpacing(8);
  layout->addWidget(result);
  layout->addLayout(buttons);

  QVBoxLayout *root = new QVBoxLayout(&window);
  root->setContentsMargins(20, 20, 20, 20);
  root->addWidget(card);

  window.show();

  QFutureWatcher<QString> *watcher = new QFutureWatcher<QString>(&window);

  auto startGenerate = [=]() {
    copyBtn->setEnabled(false);
    generateBtn->setEnabled(false);
    result->setText("⏳ Generating license...");
    QApplication::processEvents();

    QFuture<QString> future = QtConcurrent::run([]() -> QString {
      LicenseManager manager(1, 10, "");
      manager.generateLicenses();
      auto licenses = manager.getLicenses();
      if (!licenses.empty())
        return QString::fromStdString(licenses[0].license);
      return QString("❌ No license generated.");
    });

    watcher->setFuture(future);
  };

  QObject::connect(watcher, &QFutureWatcher<QString>::finished, [&]() {
    QString lic = watcher->result();
    result->setText(lic);
    copyBtn->setEnabled(true);
    generateBtn->setEnabled(true);
  });

  QObject::connect(copyBtn, &QPushButton::clicked, [=]() {
    QApplication::clipboard()->setText(result->toPlainText());
  });

  QObject::connect(generateBtn, &QPushButton::clicked, startGenerate);

  startGenerate();

  return app.exec();
}

#include "main_qt.moc"
