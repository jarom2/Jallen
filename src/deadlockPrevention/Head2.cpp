#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include "Head.hpp"
#include "Head2.hpp"

int Head2::quantity = 0;

Head2::Head2(int id) : Head(){
    //m_callback = callback;
    std::cout << "Head2 Constructor" << std::endl;
    m_mutexRight = std::shared_ptr<std::mutex>(new std::mutex);
    quantity++;
    m_id = id;
    std::cout << "Head created with id: " << m_id << std::endl;
}

void Head2::execute(std::function<void()> action){

    //if ID  is odd, lock left first
    if((m_id%2) == 1){
        auto leftLock = m_mutexLeft.lock();
        leftLock->lock();
        m_mutexRight->lock();
        //std::cout << "Head doing work: " << m_id << std::endl; 
        action();
        m_mutexRight->unlock();
        leftLock->unlock();
    }
    //if ID is even, lock right first
    else{
        m_mutexRight->lock();
        auto leftLock = m_mutexLeft.lock();
        leftLock->lock();
        //std::cout << "Head doing work: " << m_id << std::endl; 
        action();
        leftLock->unlock();
        m_mutexRight->unlock();
    }

}

std::shared_ptr<std::mutex> Head2::getRight(){
    return m_mutexRight;
}

void Head2::setLeft(std::weak_ptr<std::mutex> setter){
    m_mutexLeft = setter;
}
