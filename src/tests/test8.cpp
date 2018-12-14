#include <iostream>
#include "include.hpp"

int main()
{
#include "instantiate.hpp"

  //each insert is at head
  inner.insert("two",   2);
  inner.insert("three", 3);
  inner.insert("one",   1);

  //display
  inner.forEach([](const std::string &key, int &val){
    std::cout << val;
  });
}
