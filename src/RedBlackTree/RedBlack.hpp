#ifndef REDBLACK_HPP
#define REDBLACK_HPP

#include "KeyVal.hpp"
#include <list>
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
class RedBlack;

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
friend class RedBlack<K, V>;
public:
  /**
   * @brief Default Constructor
   */
  ListKeyValNode(){
   
  }
  bool isRed(){
  if(this->m_red){
    return true;
  }
  else{
    return false;
  }
 }
 std::shared_ptr<ListKeyValNode <K, V> > parent(){
      return this->m_parent.lock();
      //if(!(this->m_parent.lock())){
      //}
      //return nullptr;
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
    m_red  = other.m_red;
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
  std::shared_ptr<ListKeyValNode<K, V>> m_right;
  std::weak_ptr<ListKeyValNode<K, V>> m_parent;
  bool m_red = false;
};

/**
 * @class ListKeyVal
 */
template<class K, class V>
class RedBlack : public KeyVal<K, V>
{
public:
  /**
   * @brief Constructor
   * 
   * This ctor creates a valid root node
   */
  RedBlack(){
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
  RedBlack(const RedBlack &other){
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
  RedBlack &operator=(const RedBlack &other)
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
      auto root = insertInternal(key, val);
    }
  /**
   * @brief Remove an object from the associative array
   *
   * This will remove the key/salue pair from the array
   * If the key is not found, no action is taken
   *
   * Note that since a delete may change the root node, I have 
   * created an "internal"s function that returns the new root.
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
    return nullptr;
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
void printTree()
  {
    //create some working variables on stack
    //and initially populate 
    std::list<std::shared_ptr<ListKeyValNode<K, V> > > workList;
    std::list<int> levelList;
    workList.push_back(m_rootNode);
    levelList.push_back(0);

    //while more nodes exist for BFS
    while(!workList.empty())
    {
      //determine information for current vertex
      auto pCurrent = workList.front();
      auto level = levelList.front();
      auto pParent = pCurrent->parent(); 
      auto color = (pCurrent->isRed()) ? "RED" : "BLACK";

      //remove current vertex from working lists
      workList.pop_front();
      levelList.pop_front();

      //print the node
      std::cout << "{K:"<< *(pCurrent->m_key) << ", ";   //KEY
      std::cout << "C:"<< color << ", ";                 //COLOR
      std::cout << "L:"<< level << ", ";                 //LEVEL
      if(pParent) 
        std::cout << "PK:"<< *(pParent->m_key) << "}";   //PARENT KEY
      else
        std::cout << "PK:NULL}";

      //add left child to work list if valid
      if(pCurrent->m_left->m_key)
      {
        workList.push_back(pCurrent->m_left);
        levelList.push_back(level + 1);
      }

      //add right child to work list if valid
      if(pCurrent->m_right->m_key)
      {
        workList.push_back(pCurrent->m_right);
        levelList.push_back(level + 1);
      }

      //print new line when level changes
      if(!levelList.empty() && (level != levelList.front()))
        std::cout << std::endl;
    }
    //new line at end of traversal
    std::cout << std::endl;
  }
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
        rootInsert(key, val);
        return m_rootNode;
    }
    while(1){
        if(key < *(node->m_key)){
          auto temp = node;
          node = node->m_left;
          if(!(node->m_key)){
            nodeInsert(key, val, node, temp);
            balance(node);
            return m_rootNode;
          }
        }
        else if(key > *(node->m_key)){
          auto temp = node;
          node = node->m_right;
          if(!(node->m_key)){
            nodeInsert(key, val, node, temp);
            balance(node);
            return m_rootNode;
          }
        }
        else if(key == *(node->m_key)){
          *(node->m_val) = val;
          return m_rootNode;
        }
      }
    return m_rootNode;
  }

  /* Balance Function */
  virtual void balance(std::shared_ptr<ListKeyValNode <K, V> > node){
    auto p_parent = parent(node);
    auto p_uncle = uncle(node);
    auto p_grandparent = grandparent(node);
    //auto p_sibling = sibling(node);
    if(node == m_rootNode){
      node->m_red = false;
      return;
    }
    if(!(p_parent->m_red)){
      return;
    }
    if (!p_grandparent) {
      return;
    }

    //if(p_uncle){
      if(p_parent->m_red && p_uncle->m_red){
        p_parent->m_red = false;
        p_uncle->m_red = false;
        p_grandparent->m_red = true;
        balance(p_grandparent);
        return;
      }
    //}
    //Left/Right
    if((p_parent->m_red) && (p_parent == p_grandparent->m_left) && (node == p_parent->m_right)){
      p_parent->m_right = node->m_left;
      p_parent->m_right->m_parent = p_parent;

      p_grandparent->m_left = node;
      node->m_left = p_parent;

      node->m_parent = p_grandparent;
      p_parent->m_parent = node;

      auto temp1 = node;
      node = p_parent;
      p_parent = temp1;
    }

    //Right/Left
    if((p_parent->m_red) && (p_parent == p_grandparent->m_right) && (node == p_parent->m_left)){
      p_parent->m_left = node->m_right;
      p_parent->m_left->m_parent = p_parent;

      p_grandparent->m_right = node;
      node->m_right = p_parent;

      node->m_parent = p_grandparent;
      p_parent->m_parent = node;

      auto temp1 = node;
      node = p_parent;
      p_parent = temp1;
    }

    //Left/Left
    if(p_parent->m_red && (node == p_parent->m_left)){
      swapColor(p_parent, p_grandparent);

      auto oldRight = p_parent->m_right;
      auto source = parent(p_grandparent);

      p_parent->m_right = p_grandparent;
      p_grandparent->m_parent = p_parent;

      p_grandparent->m_left = oldRight;
      oldRight->m_parent = p_grandparent;

      if(source){
        if(source->m_left == p_grandparent){
          source->m_left = p_parent;
          p_parent->m_parent = source;
        }
        else if(source->m_right == p_grandparent){
          source->m_right = p_parent;
          p_parent->m_parent = source;
        }
      }
      if(p_grandparent == m_rootNode){
        m_rootNode = p_parent;
      }
    }

    //Right/Right
    if(p_parent->m_red && (node == p_parent->m_right)){

      swapColor(p_parent, p_grandparent);

      auto oldLeft = p_parent->m_left;
      auto source = parent(p_grandparent);

      p_parent->m_left = p_grandparent;
      p_grandparent->m_parent = p_parent;

      p_grandparent->m_right = oldLeft;
      oldLeft->m_parent = p_grandparent;

      if(source){
        if(source->m_left == p_grandparent){
          source->m_left = p_parent;
          p_parent->m_parent = source;
        }
        else if(source->m_right == p_grandparent){
          source->m_right = p_parent;
          p_parent->m_parent = source;
        }
      }
      if(p_grandparent == m_rootNode){
        m_rootNode = p_parent;
      }
    }
  }

  void swapColor(std::shared_ptr<ListKeyValNode <K, V> > parent, std::shared_ptr<ListKeyValNode <K, V> > grandparent){
    parent->m_red = false;
    grandparent->m_red = true;
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

  std::shared_ptr<ListKeyValNode <K, V> > parent(std::shared_ptr<ListKeyValNode <K, V>> node){
      if(node != m_rootNode){
        return node->m_parent.lock();
      }
      return nullptr;
  }

  std::shared_ptr<ListKeyValNode <K, V> > sibling(std::shared_ptr<ListKeyValNode <K, V>> node){
      if(node != m_rootNode){
//        if(node->m_key){
            if(node == node->m_parent.lock()->m_left){
                return node->m_parent.lock()->m_right;
            }
            if(node == node->m_parent.lock()->m_right){
                return node->m_parent.lock()->m_left;
            }
//        }
      }
      return nullptr;
  }

  std::shared_ptr<ListKeyValNode <K, V> > uncle(std::shared_ptr<ListKeyValNode <K, V>> node){
      if(node != m_rootNode){
        return sibling(node->m_parent.lock());
      }
      return nullptr;
  }

  std::shared_ptr<ListKeyValNode <K, V> > grandparent(std::shared_ptr<ListKeyValNode <K, V>> node){
      if(node != m_rootNode && node != m_rootNode->m_right && node != m_rootNode->m_left){
        return parent(node->m_parent.lock());
      }
      return nullptr;
  }


  
  std::shared_ptr<ListKeyValNode <K, V>> rootInsert(const K &key, const V &val){
      m_rootNode->m_key.reset(new K());
      *(m_rootNode->m_key) = key;
      m_rootNode->m_val.reset(new V());
      *(m_rootNode->m_val) = val;
      m_rootNode->m_red = false;
      m_rootNode->m_parent.reset();
      m_rootNode->m_left.reset(new ListKeyValNode<K, V>);
      m_rootNode->m_left->m_parent = m_rootNode;
      m_rootNode->m_right.reset(new ListKeyValNode<K, V>);
      m_rootNode->m_right->m_parent = m_rootNode;
      return m_rootNode;
  }

  std::shared_ptr<ListKeyValNode <K, V>> nodeInsert(const K &key, const V &val, std::shared_ptr<ListKeyValNode <K, V>> node, std::shared_ptr<ListKeyValNode <K, V>> parent){
    node->m_key.reset(new K());
    *(node->m_key) = key;
    node->m_val.reset(new V());
    *(node->m_val) = val;
    node->m_left.reset(new ListKeyValNode<K, V>);   
    node->m_left->m_parent = m_rootNode;  
    node->m_left->m_red = false;
    node->m_right.reset(new ListKeyValNode<K, V>);
    node->m_right->m_parent = m_rootNode;
    node->m_right->m_red = false;
    node->m_parent = parent;
    node->m_red = true;
    return node;
  }
  
  /**
   * @brief Delete a node and return new root
   *
   * @param key Key to insert
   * @return New root of node list
   */
  std::shared_ptr<ListKeyValNode <K, V> > delInternal(const K &key){
    
  }
  /** @breif Track root node */
  std::shared_ptr<ListKeyValNode<K, V> > m_rootNode;
};

#endif /* REDBLACK */

/*p_parent->m_right = p_grandparent;
      if(p_grandparent->m_parent.lock()->m_left == p_grandparent){
        p_grandparent->m_parent.lock()->m_left = p_parent;
      }
      else if(p_grandparent->m_parent.lock()->m_right == p_grandparent){
        p_grandparent->m_parent.lock()->m_right = p_parent;
      }*/
