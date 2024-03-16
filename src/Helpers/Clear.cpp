#include "Clear.h"
void Clear() {
#if defined _WIN32
  system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
  /* system("clear"); */
  std::cout << u8"\033[2J\033[1;1H";
#elif defined(__APPLE__)
  system("clear");
#endif
}
