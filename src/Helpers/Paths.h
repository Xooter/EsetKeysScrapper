#pragma once
#include "../../dependencies/json.hpp"
#include "curl/curl.h"
#include <thread>

#include "filesystem"
#include "fstream"
#include "string"
#include <chrono>
#include <format>
#include <iostream>
#include <regex>

using namespace std;
using namespace nlohmann;

string getPath();
