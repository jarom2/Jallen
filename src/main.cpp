#include <functional>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <sstream>
#include "Head.hpp"
#include "Head2.hpp"

std::mutex mtx1;
int concurrent = 0;
int total = 0;

void callback(int i){
    concurrent++;
    sleep(1);
    mtx1.lock();
    std::cout << "Number Working..." << concurrent << std::endl;
    mtx1.unlock();
    concurrent--;
    total++;
}


int main(int argc, char **argv){
    std::cout << "Program begin." << std::endl;
    const int HEADS = 100;
    const int THREADS = 500;

    //initialize vectors
    std::vector<std::shared_ptr<Head>> platter = Head::makePlatter(HEADS);
    std::cout << "initialized platter. Number of heads: " << Head2::quantity << std::endl;
    std::vector<std::shared_ptr<std::thread>> threads;

    //spawn threads
    for(int i = 0; i < THREADS; i++){
        //std::cout << "Spawning threads." << std::endl;
        threads.push_back(std::shared_ptr<std::thread> (new std::thread([&platter, i, HEADS]() {
            //std::cout << "spawning thread: " << i << std::endl;
            platter[i%HEADS]->execute([i](){callback(i);});
        })));
    }
    std::cout << "finished spawning threads." << std::endl;
    //join threads
    for(int i = 0; i < THREADS; i ++){
        threads[i]->join();
    }
    std::cout << "Concurrent: " << concurrent << std::endl;
    std::cout << "total: " << total << std::endl;
    return 0;
}