#ifndef CRYPTOSHIFT_HPP
#define CRYPTOSHIFT_HPP

#include "Crypto.hpp"

class CryptoShift : public Crypto
{
public:
  CryptoShift(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
         std::function<void(const uint8_t *data, uint32_t len)> decryptCallback);
  virtual void genKeys() override;
  virtual void destroyKeys() override;
  virtual void setKeys(const uint8_t *pubKey, uint32_t pubLen,
                       const uint8_t *priKey, uint32_t priLen) override;
  virtual bool getKeys(uint8_t **pubKey, uint32_t &pubLen,
                       uint8_t **priKey, uint32_t &priLen) override;
  virtual bool encrypt(const uint8_t *data, uint32_t len) override;
  virtual bool decrypt(const uint8_t *data, uint32_t len) override;

protected:
  uint8_t key;
};
#endif
