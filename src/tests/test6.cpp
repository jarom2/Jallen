#include <iostream>
#include "include.hpp"

int main()
{
#include "instantiate.hpp"

  inner.insert("one",   1);
  inner.insert("two",   2);
  inner.insert("three", 3);
  //increment val
  inner.forEach([](const std::string &key, int &val){
    val++;
  });
  //display
  inner.forEach([](const std::string &key, int &val){
    std::cout << val;
  });
}
