#include "Paths.h"

string getPath() {
  filesystem::path path = filesystem::current_path();
  path = path.parent_path().append("data/");

  if (!filesystem::exists(path)) {
    try {
      filesystem::create_directory(path);
    } catch (const std::exception &e) {
      std::cerr << "Error al crear el directorio: " << e.what() << std::endl;
    }
  }

  return path.string();
}
