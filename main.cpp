#include "dependencies/cxxopts.hpp"
#include "src/LicenseManager.h"

#define version "1.1"

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
  int domainLenght = 10;
  string proxyFile = "";

  options.add_options()("h,help", "Show help")("v,version", "Show version")(
      "n,number", "Number of licenses",
      cxxopts::value<int>(numLicenses)->default_value("1"))(
      "l,length", "Domain lenght for temporal mails",
      cxxopts::value<int>(domainLenght)->default_value("10"))(
      "p,proxy", "Proxy List file (protocol://ip:port)",
      cxxopts::value<string>(proxyFile));

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

    numLicenses = numLicenses < 1 ? 1 : numLicenses;

    domainLenght = domainLenght > 25 ? 25 : domainLenght;
    domainLenght = domainLenght < 5 ? 5 : domainLenght;

    LicenseManager licenseManager(numLicenses, domainLenght, proxyFile);

    licenseManager.generateLicenses();

    cout << endl << GREEN << "---" << RESET << endl;
    cout << GREEN << "Licenses generated successfully" << RESET << endl << endl;
    licenseManager.showAllLicenses();

  } catch (const bad_exception &e) {
    cout << "Error parsing options: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
