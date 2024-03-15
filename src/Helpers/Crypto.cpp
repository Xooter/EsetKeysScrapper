#include "Crypto.h"

// 43 for PKCE and 32 for State
string Crypto::generateRandomString(int lenght, bool specialChars) {
  string address;
  string alphabet =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

  if (specialChars) {
    alphabet += "-_";
  }

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, alphabet.size() - 1);

  for (int i = 0; i < lenght; ++i) {
    int indiceAleatorio = dis(gen);
    address += alphabet[indiceAleatorio];
  }

  return address;
}

string Crypto::base64_url_encode(const string &input) {
  // Crear un objeto BIO para la codificación Base64
  BIO *bio = BIO_new(BIO_s_mem());
  BIO *base64_bio = BIO_new(BIO_f_base64());

  // Evitar la inserción de nueva línea al final
  BIO_set_flags(base64_bio, BIO_FLAGS_BASE64_NO_NL);

  // Conectar los BIOs
  BIO_push(base64_bio, bio);

  // Escribir los datos en el BIO de codificación Base64
  BIO_write(base64_bio, input.c_str(), input.length());
  BIO_flush(base64_bio);

  // Leer los datos codificados del BIO
  BUF_MEM *bufPtr;
  BIO_get_mem_ptr(base64_bio, &bufPtr);

  // Convertir los datos codificados a una cadena C++
  std::string encoded(bufPtr->data, bufPtr->length);

  // Liberar los BIOs
  BIO_free_all(base64_bio);

  // Reemplazar caracteres especiales de Base64URL
  size_t pos;
  while ((pos = encoded.find('+')) != std::string::npos) {
    encoded.replace(pos, 1, "-");
  }
  while ((pos = encoded.find('/')) != std::string::npos) {
    encoded.replace(pos, 1, "_");
  }
  // Eliminar los caracteres de relleno "=" al final
  while (!encoded.empty() && encoded.back() == '=') {
    encoded.pop_back();
  }

  return encoded;
}

std::string Crypto::sha256(const std::string &input) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, input.c_str(), input.length());
  SHA256_Final(hash, &sha256);

  std::stringstream ss;
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    ss << std::hex << std::setw(2) << std::setfill('0')
       << static_cast<int>(hash[i]);
  }

  return ss.str();
}

/* string Crypto::sha256(const string str) { */
/**/
/**/
/**/
/*   unsigned char hash[SHA256_DIGEST_LENGTH]; */
/**/
/*   EVP_MD_CTX *sha256_ctx = EVP_MD_CTX_create(); */
/*   EVP_MD *sha256_md = EVP_MD_fetch(NULL, "SHA256", NULL); */
/**/
/*   EVP_DigestInit_ex(sha256_ctx, sha256_md, NULL); */
/*   EVP_DigestUpdate(sha256_ctx, str.c_str(), str.size()); */
/*   EVP_DigestFinal_ex(sha256_ctx, hash, NULL); */
/**/
/*   EVP_MD_CTX_free(sha256_ctx); */
/*   EVP_MD_free(sha256_md); */
/**/
/*   stringstream ss; */
/**/
/*   for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) { */
/*     ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]); */
/*   } */
/**/
/*   return ss.str(); */
/* } */

std::string Crypto::sha256_base64url(const std::string &input) {
  // Calcular el hash SHA-256
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256((const unsigned char *)input.c_str(), input.length(), hash);

  // Convertir el hash a una cadena hexadecimal
  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
  }
  std::string hexHash = ss.str();

  // Decodificar el hash hexadecimal a una secuencia de bytes
  std::vector<unsigned char> bytes;
  for (size_t i = 0; i < hexHash.length(); i += 2) {
    unsigned int byte;
    std::istringstream(hexHash.substr(i, 2)) >> std::hex >> byte;
    bytes.push_back(static_cast<unsigned char>(byte));
  }

  // Crear un objeto BIO para la codificación Base64
  BIO *bio = BIO_new(BIO_s_mem());
  BIO *base64_bio = BIO_new(BIO_f_base64());

  // Evitar la inserción de nueva línea al final
  BIO_set_flags(base64_bio, BIO_FLAGS_BASE64_NO_NL);

  // Conectar los BIOs
  BIO_push(base64_bio, bio);

  // Escribir los datos en el BIO de codificación Base64
  BIO_write(base64_bio, bytes.data(), bytes.size());
  BIO_flush(base64_bio);

  // Leer los datos codificados del BIO
  BUF_MEM *bufPtr;
  BIO_get_mem_ptr(base64_bio, &bufPtr);

  // Convertir los datos codificados a una cadena C++
  std::string encoded(bufPtr->data, bufPtr->length);

  // Liberar los BIOs
  BIO_free_all(base64_bio);

  // Reemplazar caracteres especiales de Base64URL
  size_t pos;
  while ((pos = encoded.find('+')) != std::string::npos) {
    encoded.replace(pos, 1, "-");
  }
  while ((pos = encoded.find('/')) != std::string::npos) {
    encoded.replace(pos, 1, "_");
  }
  // Eliminar los caracteres de relleno "=" al final
  while (!encoded.empty() && encoded.back() == '=') {
    encoded.pop_back();
  }

  return encoded;
}
