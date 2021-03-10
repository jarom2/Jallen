#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include "Head.hpp"
#include "Head2.hpp"
Head::Head(){

}

std::vector<std::shared_ptr<Head>> Head::makePlatter(int numHeads){
    std::weak_ptr<std::mutex> left;
    std::vector<std::shared_ptr<Head2>> temp;
    std::cout << "populating platter" << std::endl;
    for(int i = 0; i < numHeads; i++){
        temp.push_back(std::shared_ptr<Head2>(new Head2(i)));
    }
    std::cout << "platter populated" << std::endl;
    for(int i = 0; i < numHeads; i++){
        if(0 == i){
            std::cout << "entering initial." << std::endl;
            left = temp[numHeads-1]->getRight();
            temp[i]->setLeft(left);
            std::cout << "initial done." << std::endl;
        }
        else{
            left = temp[i-1]->getRight();
            temp[i]->setLeft(left);
        } 
    }
    std::cout << "pointers connected" << std::endl;
    return std::vector<std::shared_ptr<Head>>(temp.begin(), temp.end());
}

