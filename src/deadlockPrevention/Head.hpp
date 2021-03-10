#ifndef HEAD_HPP
#define HEAD_HPP

#include <functional>
#include <memory>
#include <vector>

/**
 * @class Head
 *
 * This class is an ABC that represents a read write
 * head on a hard drive platter. The pupose of this
 * class is to ensure:
 *   1. No two adjacent heads are active concurrently
 *   2. Allow the maximum amout of concurrency possible
 */
class Head
{
public:
  /**
   * @brief Execute an operation on this head
   *
   * This method must be implemented by any concrete class.  It must
   * ensure that both of the the adjacent heads are not currenty active.
   * At that point, it can execute the function object and then release
   * any resources preventing the adjacent heads from acting.
   *
   * This function MUST be thread safe.  Any number of threads may call this 
   * function any number of times and behavior must be correct.
   *
   * @param action Function object that contains read/write/other action
   */
  virtual void execute(std::function<void()> action) = 0;
  /**
   * @breif Static factory method
   *
   * This static method will create a number of head objects, connect
   * each of the heads in a way that they can lock out their right/left
   * neighbor, and return shared pointers to each created head in a vector.
   */
  static std::vector<std::shared_ptr<Head> > makePlatter(int numHeads);
protected:
  /**
  * @brief Protected constructor
  *
  * Since it is required that a Head have knowledge of their right/left
  * neighbor, it is wrong to create one in isolation.  This will force
  * a user of this package to use the static factory method.
  */
  Head();
};

#endif /* HEAD_HPP */
