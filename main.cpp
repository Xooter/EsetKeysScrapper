#include "dependencies/cxxopts.hpp"
#include "src/Eset/Eset.h"
#include "src/TempMail/TempMail.h"
#include <exception>

#define version "1.0"

using namespace std;

void saveGeneratedData(const string &filename,
                       vector<unique_ptr<TempMail>> &tempMails,
                       vector<string> &licenses) {
  ofstream file(filename);

  if (file.is_open()) {
    for (int i = 0; i < tempMails.size(); i++) {
      file << tempMails[i]->getEmail() << "," << licenses[i] << endl;
    }

    file.close();
  }
}

vector<unique_ptr<TempMail>> tempMails;
vector<string> licenses;

int main(int argc, char *argv[]) {

  cout << CYAN << R"(
  ______          _   _  __               _____                                      
 |  ____|        | | | |/ /              / ____|                                     
 | |__   ___  ___| |_| ' / ___ _   _ ___| (___   ___ _ __ __ _ _ __  _ __   ___ _ __ 
 |  __| / __|/ _ | __|  < / _ | | | / __|\___ \ / __| '__/ _` | '_ \| '_ \ / _ | '__|
 | |____\__ |  __| |_| . |  __| |_| \__ \____) | (__| | | (_| | |_) | |_) |  __| |   
 |______|___/\___|\__|_|\_\___|\__, |___|_____/ \___|_|  \__,_| .__/| .__/ \___|_|   
                                __/ |                         | |   | |              
                               |___/                          |_|   |_|              


  )" << RESET
       << endl;

  cxxopts::Options options("EsetKeysScrapper",
                           "A tool for extracting free licenses from Eset "
                           "NOD32 antivirus accounts through web scraping");

  int numLicenses = 1;
  int domainLenght = 15;

  options.add_options()("h,help", "Show help")("v,version", "Show version")(
      "n,number", "Number of licenses",
      cxxopts::value<int>(numLicenses)->default_value("1"))(
      "l,length", "Domain lenght for temporal mails",
      cxxopts::value<int>(domainLenght)->default_value("15"));

  try {
    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      cout << options.help() << std::endl;
      return 0;
    }

    if (result.count("version")) {
      cout << "EsetKeysScrapper" << LGREEN << "v" << version << std::endl;
      return 0;
    }

    cout << RED << "DISCLAIMER:" << endl
         << "This tool is for educational purposes only. " << endl
         << "Use at your own risk. I am not responsible for any damage caused "
            "by the "
            "use of this tool."
         << RESET << endl;
    cout << GREEN << "\tXooter." << RESET << endl << endl;

    for (int i = tempMails.size(); i < numLicenses; ++i) {
      tempMails.emplace_back(make_unique<TempMail>(domainLenght));
    }

    for (auto &tempMail : tempMails) {
      Eset eset(tempMail->getEmail());

      eset.CreateAccount();
      this_thread::sleep_for(chrono::milliseconds(100));

      bool activated = false;

      cout << endl << YELLOW << "Waiting to verification code" << endl;

      do {
        tempMail->getMessages();
        for (auto &message : tempMail->messages) {
          if (message.from == "info@product.eset.com" &&
              message.subject == "Account confirmation") {
            tempMail->readMessage(message.id);
            if (eset.ConfirmRegistration(message.body)) {
              activated = true;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
          }
        }
      } while (!activated);

      int attempts = 0;
      while (!eset.GetLicense() && attempts < 5) {
        cout << YELLOW << "Retrying to get license" << endl;
        attempts++;
      }

      licenses.push_back(eset.license);
    }

    saveGeneratedData(getPath() + "data.csv", tempMails, licenses);

  } catch (const bad_exception &e) {
    cout << "Error parsing options: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
