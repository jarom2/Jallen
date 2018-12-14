#include <iostream>
#include "include.hpp"

int main()
{
#include "instantiate.hpp"

  //copy ctor
  auto newObj = outer;
  //assign operator
  newObj = outer;
  std::cout << "success" << std::endl;
}
