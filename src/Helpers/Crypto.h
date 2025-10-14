#pragma once
#include "../../dependencies/colors.h"
#include "../../dependencies/json.hpp"
#include <curl/curl.h>

#include "filesystem"
#include "fstream"
#include "string"
#include <chrono>
#include <format>
#include <iomanip>
#include <iostream>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <regex>
#include <thread>

#include <algorithm>
#include <random>

using namespace std;
using namespace nlohmann;

namespace Crypto {

// 43
string generateRandomString(int lenght = 56, bool specialChars = true);

string base64_url_encode(const string &input);

string sha256_base64url(const std::string &input);

string sha256(const string &input);
} // namespace Crypto
