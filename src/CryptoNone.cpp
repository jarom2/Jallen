#include "CryptoNone.hpp"
#include <iostream>

void CryptoNone::genKeys(){
  key = 0;
}

void CryptoNone::destroyKeys(){
  key = 0;
}

void CryptoNone::setKeys(const uint8_t *pubKey, uint32_t pubLen,
                         const uint8_t *priKey, uint32_t priLen)
{
  key = (*pubKey);
}

bool CryptoNone::getKeys(uint8_t **pubKey, uint32_t &pubLen,
                         uint8_t **priKey, uint32_t &priLen)
{
  (*pubKey) = new uint8_t[1];
  (*pubKey)[0] = key;
  pubLen = 1;
  priLen = 0;
  return true;
}
bool CryptoNone::encrypt(const uint8_t *data, uint32_t len){
  uint8_t *temp = new uint8_t[len];
  for(int i = 0; i < len; i++){
    temp[i] = data[i];
  }
  m_encryptCallback(temp, len);
}

bool CryptoNone::decrypt(const uint8_t *data, uint32_t len){
  uint8_t *temp = new uint8_t[len];
  for(int i = 0; i < len; i++){
    temp[i] = data[i];
  }
  m_decryptCallback(temp, len);
  return true;
}
  

  