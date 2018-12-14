#include <iostream>
#include "BinTree.hpp"
#include "KeyVal.hpp"

int main(){
    BinTree<std::string, int> bst;
 
    bst.insert("5", 5);
    bst.insert("3", 3);
    bst.insert("6", 6);
    BinTree<std::string, int> bst3(bst);
    BinTree<std::string, int> bst2;
    bst2 = bst;
    bst.del("5");
    bst2.forEach([](const std::string &key, const int &val){std::cout << "Key: " << key <<std::endl;
                         std::cout << "Val: " << val << std::endl;});
    
    auto test = bst2.get("6");
    *(test) = 420;
    std::cout << "Before exit." << std::endl;
    return 0;
}