#include <iostream>
#include "include.hpp"

int main()
{
#include "instantiate.hpp"

  //populate
  inner.insert("one", 1);
  inner.insert("two", 2);
  inner.del("two");
  inner.del("two"); //second delete; key doesn't exist
  auto ptr = inner.get("two");
  if(!ptr)
    std::cout << "success";
}
