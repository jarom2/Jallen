#ifndef BINTREE_HPP
#define BINTREE_HPP

#include "KeyVal.hpp"
#include <iostream>

/*********************************************************************
 * This is an example header without any implementation that you
 * may find useful in your effort to create an associative array.
 *
 * You are not required to conform to this header in any way, you are
 * only required to conform to the overall interface header provided
 * in KeyVal.hpp.
 *
 * However, there are helpful hints contained here that may make both
 * this PEX and future PEXes which build on the base interface easier
 * for you to implement.  A careful study of this architecture is 
 * highly recommended. *
 ********************************************************************/

//forward declaration
template<class K, class V>
class BinTree;

/**
 * @class ListKeyValNode
 *
 * The purpose of this class is to store key value pairs in a sorted list.
 * Note that all methods and members are private and only accessible by the
 * friend class ListKeyVal.
 *
 * The reason that the ListKeyValNode class and the ListKeyVal class are 
 * separate is to make usage cleaner.  We must be able to create ListKeyVal 
 * objects on the stack just as we would any other object.  Since additions and
 * deletions to the list may change the root node, it is not effective to 
 * create a root node as the base object
 *
 * This separate private class also allows us to remove some default constuctors
 * and use only deep copy assignment in this class while exposing all options
 * in the public class.
 */
template<class K, class V>
class ListKeyValNode : public std::enable_shared_from_this<ListKeyValNode<K, V> >
{
friend class BinTree<K, V>;
public:
  /**
   * @brief Default Constructor
   */
  ListKeyValNode(){
   
  }
  /**
   * @brief Remove the copy constructor
   */
  ListKeyValNode(const ListKeyValNode &other) = delete;
  /**
   * @brief provide an assignment operator which deep copies our
   * data structure
   *
   * @param other Ref to RHS object; deep copy this structure
   * @return ListKeyNodeVal ref for chaining assignments
   */
  ListKeyValNode &operator=(const ListKeyValNode &other)
  {
    if(other.m_key){
      m_key.reset(new K());
      *m_key = *other.m_key;

      m_val.reset(new V());
      *m_val = *other.m_val;

      m_left.reset(new ListKeyValNode());
      m_right.reset(new ListKeyValNode());

      *m_left = *other.m_left;    
      *m_right = *other.m_right;

    }
    return *this;
    
  }
  /** @brief Ptr to key; may be null for last item in list */
  std::shared_ptr<K> m_key;
  /** @brief Ptr to value; may be null for last item in list */
  std::shared_ptr<V> m_val;
  /** @brief Ptr to next node in list; may be null for last item in list */
  std::shared_ptr<ListKeyValNode> m_left;
  /** @brief Weak ptr to prev node in list; may be null for first item in list 
       note that weak ref is used to avoid mem leak islands*/
  std::shared_ptr<ListKeyValNode<K, V> > m_right;
};

/**
 * @class ListKeyVal
 */
template<class K, class V>
class BinTree : public KeyVal<K, V>
{
public:
  /**
   * @brief Constructor
   * 
   * This ctor creates a valid root node
   */
  BinTree(){
    m_rootNode.reset(new ListKeyValNode<K, V>());
    m_rootNode->m_left.reset(new ListKeyValNode<K, V>());
    m_rootNode->m_right.reset(new ListKeyValNode<K, V>());
  }
  /**
   * @brief Copy ctor
   *
   * Creates a deep copy of entire data structure
   *
   * @param other Data structure to copy
   */
  BinTree(const BinTree &other){
    m_rootNode.reset(new ListKeyValNode<K, V>());
    m_rootNode->m_left.reset(new ListKeyValNode<K, V>());
    m_rootNode->m_right.reset(new ListKeyValNode<K, V>());
    if(other.m_rootNode->m_key){
    *(m_rootNode) = *(other.m_rootNode);
    }
  }
  /**
   * @brief Assignment operator
   *
   * Creates a deep copy of entire data structure
   *
   * @param other Data structure to copy
   * @return ref to populated object for assignment chaining
   */
  BinTree &operator=(const BinTree &other)
  {
    *(m_rootNode) = *(other.m_rootNode); 
  }
  /**
   * @brief Insert an object
   *
   * This will place a COPY of the val object
   * into the associative array
   *
   * Note that since an insert may change the root node, I have 
   * created an "internal" function that returns the new root.
   * this function will call the internal version and then reset
   * the root node if needed.  This model will make follow on
   * PEXs where recursion is required more clean/understandable.
   *
   * @param key Key associated with value
   * @param val Value which is stored at location key
   */
  virtual void insert(const K &key, const V &val) override{
      insertInternal(key, val);
    }
  /**
   * @brief Remove an object from the associative array
   *
   * This will remove the key/value pair from the array
   * If the key is not found, no action is taken
   *
   * Note that since a delete may change the root node, I have 
   * created an "internal" function that returns the new root.
   * this function will call the internal version and then reset
   * the root node if needed.  This model will make follow on
   * PEXs where recursion is required more clean/understandable.
   *
   * @param key Key for which key/val pair is removed
   */
  virtual void del(const K &key) override {
    delInternal(key);
  }
  /**
   * @brief Get a pointer to value
   *
   * Given a key, a shared_ptr to a value is returned.
   * note that if the key did not exist, then the returned
   * ptr is not valid
   *
   * @param key Key for which value is returned
   * @return ptr to value if key existed
   */
  virtual std::shared_ptr<V> get(const K &key) override{
    std::shared_ptr<V> value;
    auto node = m_rootNode;
    while(node->m_key){
      if((*node->m_key) == key){
        value = node->m_val;
        return value;
      }
      else if((*node->m_key) > key){
        node = node->m_left;
      }
      else if((*node->m_key) < key){
        node = node->m_right;
      }
    }
    std::cout << "the key did not exist" << std::endl;
    return 0;
  }
  /**
   * @brief Execute callback for each entry
   *
   * Rather than force students to create an inner iterator class,
   * this functiona allows a callback function to be executed for
   * every item in the associative array.  Note that callbacks should
   * be called in order of keys sorted least to greatest
   *
   * @param callback Function to be called with each item in the associative array
   */
  virtual void forEach(std::function<void(const K &key, V &val)> callback) override {
    if(m_rootNode->m_key){
      if(m_rootNode->m_left->m_key){
        helperFunction(callback, m_rootNode->m_left);
      }
      callback(*(m_rootNode->m_key), *(m_rootNode->m_val));
      if(m_rootNode->m_right->m_key){
        helperFunction(callback, m_rootNode->m_right);
      }
    }
  }
public:
  /**
   * @brief Insert a node and return new root
   *
   * @param key Key to insert
   * @param val Value associated with key
   * @return New root of node list
   */
  std::shared_ptr<ListKeyValNode <K, V> > insertInternal(const K &key, const V &val)
  {
    auto node = m_rootNode;
    if(!(node->m_key)){
      node->m_key.reset(new K());
      *(node->m_key) = key;
      node->m_val.reset(new V());
      *(node->m_val) = val;
    }
    else{
      while(1){
        if(key < *(node->m_key)){
          node = node->m_left;
          if(!(node->m_key)){
            node->m_key.reset(new K());
            *(node->m_key) = key;
            node->m_val.reset(new V());
            *(node->m_val) = val;
            node->m_left.reset(new ListKeyValNode<K, V>);     
            node->m_right.reset(new ListKeyValNode<K, V>);
            break;
          }
        }
        else if(key > *(node->m_key)){
          node = node->m_right;
          if(!(node->m_key)){
            node->m_key.reset(new K());
            *(node->m_key) = key;
            node->m_val.reset(new V());
            *(node->m_val) = val;
            node->m_left.reset(new ListKeyValNode<K, V>);     
            node->m_right.reset(new ListKeyValNode<K, V>);
            break;
          }
        }
        else if(key == *(node->m_key)){
          *(node->m_val) = val;
          break;
        }
      }
    }
    return m_rootNode;
  }

  void helperFunction(std::function<void(const K &key, V &val)> callback, std::shared_ptr<ListKeyValNode <K, V>> node){
    if(node->m_left->m_key){
      helperFunction(callback, node->m_left);
    }
    callback(*(node->m_key), *(node->m_val));
    if(node->m_right->m_key){
      helperFunction(callback, node->m_right);
    }
  }

  
  /**
   * @brief Delete a node and return new root
   *
   * @param key Key to insert
   * @return New root of node list
   */
  std::shared_ptr<ListKeyValNode <K, V> > delInternal(const K &key){
    auto node = m_rootNode;
    while(node->m_key){
      if(key == *(node->m_key)){
        //no children delete
        if(!(node->m_left->m_key) && !(node->m_right->m_key)){
          node->m_val.reset();
          node->m_key.reset();
          node->m_left.reset();
          node->m_right.reset();
          return m_rootNode;
        }
        //left child, no right delete
        else if((node->m_left->m_key) && !(node->m_right->m_key)){
          auto temp = node;
          temp = temp->m_left;
          node->m_left = temp->m_left;
          node->m_right = temp->m_right;
          *(node->m_val) = *(temp->m_val);
          *(node->m_key) = *(temp->m_key);
          temp->m_key.reset();
          temp->m_val.reset();
          temp->m_left.reset();
          temp->m_right.reset();
          return m_rootNode;
        }
        //right child, no left delete
        else if(!(node->m_left->m_key) && (node->m_right->m_key)){
          auto temp = node;
          temp = temp->m_right;
          node->m_left = temp->m_left;
          node->m_right = temp->m_right;
          *(node->m_val) = *(temp->m_val);
          *(node->m_key) = *(temp->m_key);
          temp->m_key.reset();
          temp->m_val.reset();
          temp->m_left.reset();
          temp->m_right.reset();
          return m_rootNode;
        }
        //both children delete
        else if((node->m_left->m_key) && (node->m_right->m_key)){
          auto temp = node;
          temp = temp->m_right;
          while(temp->m_left->m_key){
            temp = temp->m_left;
          }
          *(node->m_key) = *(temp->m_key);
          *(node->m_val) = *(temp->m_val);
          node->m_right->m_left = temp;
          temp->m_left.reset();
          temp->m_right.reset();
          temp->m_val.reset();
          temp->m_key.reset();
          return m_rootNode;
        }
      }
      else if(key < *(node->m_key)){
        node = node->m_left;
      }
      else if(key > *(node->m_key)){
        node = node->m_right;
      }
    }
    return m_rootNode;
  }
  /** @breif Track root node */
  std::shared_ptr<ListKeyValNode<K, V> > m_rootNode;
};

#endif /* BINTREE */
