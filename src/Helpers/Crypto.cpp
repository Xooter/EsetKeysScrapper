#include "Crypto.h"

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
  BIO *bio = BIO_new(BIO_s_mem());
  BIO *base64_bio = BIO_new(BIO_f_base64());

  BIO_set_flags(base64_bio, BIO_FLAGS_BASE64_NO_NL);

  BIO_push(base64_bio, bio);

  BIO_write(base64_bio, input.c_str(), input.length());
  BIO_flush(base64_bio);

  BUF_MEM *bufPtr;
  BIO_get_mem_ptr(base64_bio, &bufPtr);

  std::string encoded(bufPtr->data, bufPtr->length);

  BIO_free_all(base64_bio);

  size_t pos;
  while ((pos = encoded.find('+')) != std::string::npos) {
    encoded.replace(pos, 1, "-");
  }
  while ((pos = encoded.find('/')) != std::string::npos) {
    encoded.replace(pos, 1, "_");
  }
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

std::string Crypto::sha256_base64url(const std::string &input) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256((const unsigned char *)input.c_str(), input.length(), hash);

  std::stringstream ss;
  ss << std::hex << std::setfill('0');
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
  }
  std::string hexHash = ss.str();

  std::vector<unsigned char> bytes;
  for (size_t i = 0; i < hexHash.length(); i += 2) {
    unsigned int byte;
    std::istringstream(hexHash.substr(i, 2)) >> std::hex >> byte;
    bytes.push_back(static_cast<unsigned char>(byte));
  }

  BIO *bio = BIO_new(BIO_s_mem());
  BIO *base64_bio = BIO_new(BIO_f_base64());

  BIO_set_flags(base64_bio, BIO_FLAGS_BASE64_NO_NL);

  BIO_push(base64_bio, bio);

  BIO_write(base64_bio, bytes.data(), bytes.size());
  BIO_flush(base64_bio);

  BUF_MEM *bufPtr;
  BIO_get_mem_ptr(base64_bio, &bufPtr);

  std::string encoded(bufPtr->data, bufPtr->length);

  BIO_free_all(base64_bio);

  size_t pos;
  while ((pos = encoded.find('+')) != std::string::npos) {
    encoded.replace(pos, 1, "-");
  }
  while ((pos = encoded.find('/')) != std::string::npos) {
    encoded.replace(pos, 1, "_");
  }
  while (!encoded.empty() && encoded.back() == '=') {
    encoded.pop_back();
  }

  return encoded;
}
