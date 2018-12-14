#ifndef CRYPTO_HPP
#define CRYPTO_HPP

#include <functional>
#include <memory>

/**
 * @enum Algorithm
 *
 * The purpose of this enumeration is to inform the
 * static factory method which crypto transform should be
 * used for when creating a crypto object.
 */
enum class Algorithm
{
  eNONE,
  eCAESAR,
  eAES,
  eRSA,
};

/**
 * @class Crypto
 *
 * The purpose of this class is to serve as an abstract base class for many
 * cryptologic transforms.
 */
class Crypto
{
public:
  /**
   * @brief Constructor
   *
   * This function creates/initializes this oject.
   * The callbacks are used by the underlying
   * algorithm to pass back processed data to the using logic.
   *
   * NOTE: There is no correlation between the amount of data passed into the algorithm
   * and the amount returned from it.  Likewise, there is no required correlation between
   * the number of times data is passed into the algorithm and the number of times callbacks
   * are called.
   *
   * @param encryptCallback Callback function for returning encrypted data
   * @param decryptCallback Callback function for returning decrypted data
   */
  Crypto(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
         std::function<void(const uint8_t *data, uint32_t len)> decryptCallback);
  /**
   * @brief Generate new crypto keys
   *
   * This function must be implemented by all derived classes.
   * It signals the underlying algorithm that it should generate
   * and store keys needed for that algorithm.
   */
  virtual void genKeys() = 0;
  /**
   * @brief Get the keys and return them in a standardized form
   *
   * This function must be implemented by all derived classes.
   * It will allocate memory, and then populate that memory with
   * keys in a way that can be generically stored/passed by using
   * programs.
   *
   * @param pubKey Pointer to array of bytes containing public key
   * @param pubLen Length of the public key
   * @param priKey Pointer to array of bytes containing private key
   * @param priLen Length of the private key
   * @return Indication of valid keys present in response
   */
  virtual bool getKeys(uint8_t **pubKey, uint32_t &pubLen,
                       uint8_t **priKey, uint32_t &priLen) = 0;
  /**
   * @brief Set the keys to be used in underlying transform
   *
   * This function must be implemented by all derived classes.
   * It will pass a representation of needed keys to derived
   * algorithms.
   *
   * @param pubKey Pointer to bytes containing public key
   * @param pubLen Length of the public key
   * @param priKey Pointer to bytes containing private key
   * @param priLen Length of the private key
   */
  virtual void setKeys(const uint8_t *pubKey, uint32_t pubLen,
                       const uint8_t *priKey, uint32_t priLen) = 0;
  /**
   * @brief Destroy keys and free resources
   *
   * This function must be implemented by all derived classes.
   * It will inform the derived algorithm that it should "forget"
   * any created keys and free resources associated with them.
   * If keys are not present, it should have no action.
   */
  virtual void destroyKeys() = 0;
  /**
   * @brief Encrypt raw data buffer (PT to CT)
   *
   * This function will pass a data buffer for encyrption
   * to the underlying algorithm.  There should be no restrictions
   * on the length of data that is passed to the algorithm.
   *
   * @param data Buffer of data to be encrypted
   * @param len Length of data buffer
   * @return bool Success or failure (transform accepted data)
   */
  virtual bool encrypt(const uint8_t *data, uint32_t len) = 0;
  /**
   * @brief Decrypt raw data buffer (CT to PT)
   *
   * This function will pass a data buffer for decyrption
   * to the underlying algorithm.  There should be no restrictions
   * on the length of data that is passed to the algorithm.
   *
   * @param data Buffer of data to be decrypted
   * @param len Length of data buffer
   * @return bool Success or failure (transform accepted data)
   */
  virtual bool decrypt(const uint8_t *data, uint32_t len) = 0;
  /**
   * @brief Create a Crypto object using the correct transform
   *
   * @param encryptCallback Callback function for returning encrypted data
   * @param decryptCallback Callback function for returning decrypted data
   * @param algorithm Enum indicationg which transform should be used
   * @return shared_ptr to newly constructed heap object
   */
  static std::shared_ptr<Crypto> cryptoFactory(std::function<void(const uint8_t *data, uint32_t len)> encryptCallback,
                                               std::function<void(const uint8_t *data, uint32_t len)> decryptCallback,
                                               Algorithm algorithm);
protected:
  /** @brief Encrypt callback function */
  std::function<void(const uint8_t *data, uint32_t len)> m_encryptCallback;
  /** @brief Decrypt callback function */
  std::function<void(const uint8_t *data, uint32_t len)> m_decryptCallback;
};

#endif /* CRYPTO_HPP */

//void Crypto::getKeys(*uint8_t **ptr){
//*ptr = &m_key;

//void Crypto::encrypt(*buff, len){
//	uint8_t *l_buf = (uint8_t *) buff