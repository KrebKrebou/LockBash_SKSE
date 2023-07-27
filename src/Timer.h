#pragma once
#include <chrono>
#include <iostream>
#include <thread>
#include <future>

int timer(int val) {
    bool Done = false;
    while (Done == false) {  
        // sleep for x seconds
        std::this_thread::sleep_for(std::chrono::milliseconds(val));
        // do something here after x seconds...
        //logger::info("waited for {} milliseconds", val);
        Done = true;
    } 
    return 0;
}