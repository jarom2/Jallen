#ifndef KEYVAL_HPP
#define KEYVAL_HPP
#include <memory>
#include <functional>
/**
*  @class KeyVal
*
*  This class is an interface to a system that can
*  arbitrarily track key/value pairs.  The abiltiy to
*  build an "associative array" will be invaluable
*  to a coder
*
*  An associative array allows the coder to fetch
*  an object by using a key.  This essentially makes
*  a content addressable data structure.
*
*  All derived classes must use this template verbatim
*
*  Notes:
*  1. Objects that are used as keys in the table
*  must have the less than operator defined.
*  2. KeyVal objects should be creatable on both the
*  stack and the heap
*  3. KeyVal objects should be assignable and copy constructible
*  and should make deep copies as appropriate
*  4. The forEach method should iterate over the structure in
*  sorted order least to greatest according to key value
*/
template<class K, class V>
class KeyVal
{
public:
/**
*  @brief Insert an object
*
*  This will place a COPY of the val object
*  into the associative array
*
*  @param key Key associated with value
*  @param val Value which is stored at location key
*/
virtual void insert(const K &key, const V &val) = 0;
/**
*  @brief Remove an object from the associative array
*
*  This will remove the key/value pair from the array
*  If the key is not found, no action is taken
*
*  @param key Key for which key/val pair is removed
*/
virtual void del(const K &key) = 0;
/**
*  @brief Get a pointer to value
*
*  Given a key, a shared_ptr to a value is returned.
*  note that if the key did not exist, then the returned
*  ptr is not valid
*
*  @param key Key for which value is returned
*  @return ptr to value if key existed
*/
virtual std::shared_ptr<V> get(const K &key) = 0;
/**
*  @brief Execute callback for each entry
*
*  Rather than force students to create an inner iterator class,
*  this functiona allows a callback function to be executed for
*  every item in the associative array.  Note that callbacks should
*  be called in order of keys sorted least to greatest
*
*  Note that keys are passed by const ref; they cannot be changed, yet
*  values are passed by non-const ref.  They can be changed and that 
change
*  should be reflected in the underlying data structure.
*
*  @param callback Function to be called with each item in the 
associative array
*/
virtual void forEach(std::function<void(const K &key, V &val)> callback) = 0;
};
#endif /* KEYVAL_HPP */