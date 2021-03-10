#ifndef CRYPTONONE_HPP
#define CRYPTONONE_HPP
#include "Crypto.hpp"

class CryptoNone : public Crypto
{
public:
  CryptoNone(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
		      std::function<void(const uint8_t *data, uint32_t len)> decryptCallback)
			: Crypto(encryptCallback, decryptCallback) {}
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
