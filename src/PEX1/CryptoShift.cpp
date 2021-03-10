#include "CryptoShift.hpp"
#include <random>
#include <ctime>
#include <cstdlib>

CryptoShift::CryptoShift(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
         std::function<void(const uint8_t *data, uint32_t len)> decryptCallback)
    : Crypto(encryptCallback, decryptCallback) {}

void CryptoShift::genKeys(){
  std::srand(std::time(0));
  key = std::rand();
  /*std::random_device device;
  std::mt19937 gen(rnd());
  std::unifirm_int distribution<uint8_t> dis(1,255);
  key = dis(gen);*/
}

void CryptoShift::destroyKeys(){
  key = 0; 
}

void CryptoShift::setKeys(const uint8_t *pubKey, uint32_t pubLen,
             const uint8_t *priKey, uint32_t priLen){
  key = (*pubKey);
};

bool CryptoShift::getKeys(uint8_t **pubKey, uint32_t &pubLen,
			  uint8_t **priKey, uint32_t &priLen){
  (*pubKey) = new uint8_t[1];
  (*pubKey)[0] = key;
  
  pubLen = 1;
  priLen = 0;
  return true;
}

bool CryptoShift::encrypt(const uint8_t *data, uint32_t len)
{
  uint8_t *temp = new uint8_t[len];
  for(int i = 0; i < len; i++){
    temp[i] = data[i] + key;
  }
  m_encryptCallback(temp, len);
  return true;
}

bool CryptoShift::decrypt(const uint8_t *data, uint32_t len)
{
  uint8_t *temp = new uint8_t[len];
  for(int i = 0; i < len; i++){
    temp[i] = data[i] - key;
  }
  m_decryptCallback(temp, len);
  return true;
}
  
  
