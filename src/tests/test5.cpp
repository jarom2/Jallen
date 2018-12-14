#include <iostream>
#include "include.hpp"

int main()
{
#include "instantiate.hpp"

  //populate
  inner.insert("one", 1);
  inner.insert("two", 2);
  *(inner.get("one")) = 3;
  std::cout << *(inner.get("one"));
}
