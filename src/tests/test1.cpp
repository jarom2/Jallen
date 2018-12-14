#include <iostream>
#include "include.hpp"

int main()
{
#include "instantiate.hpp"

  inner.insert("one",   1);
  inner.insert("two",   2);
  inner.insert("three", 3);
  inner.del("two");
  *(inner.get("three")) = 4;
  inner.forEach([](const std::string &key, int &val){
    std::cout << key << val;
  });
}
