#include "ListKeyVal.hpp"
#include <string>
#include <iostream>

int main()
{
  ListKeyVal<std::string, int> list;
  list.insert("one",   1);
  list.insert("two",   2);
  list.insert("three", 3);
  list.del("two");
  std::cout << "before exit" << std::endl;
  return 0;
}