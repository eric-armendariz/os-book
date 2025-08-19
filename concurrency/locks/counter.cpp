#include <iostream>
#include <stdio.h>
#include <mutex>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <time.h>

int count = 0;
int badCount = 0;
std::mutex countMutex;

void incrementCounter() {
    countMutex.lock();
    for (int i = 0; i < 1e8; i++) {
        count++;
    }
    countMutex.unlock();
}

void badIncrementCounter() {
    for (int i = 0; i < 1e8; i++) {
        badCount++;
    }
}

void printTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ) % 1000;
    std::tm* timeinfo = std::localtime(&time_t);
    std::cout << "Time: " << std::put_time(timeinfo, "%H:%M:%S");
    std::cout << "." << std::setfill('0') << std::setw(3) << ms.count() << std::endl;
}

int main() {
    printTime();
    std::thread t1(incrementCounter);
    std::thread t2(incrementCounter);
    std::thread t3(incrementCounter);

    t1.join();
    t2.join();
    t3.join();
    printTime();
    std::cout << "Final count: " << count << std::endl;


    std::thread t4(badIncrementCounter);
    std::thread t5(badIncrementCounter);
    std::thread t6(badIncrementCounter);

    t4.join();
    t5.join();
    t6.join();
    printTime();
    std::cout << "Final bad count: " << badCount << std::endl;
    return 0;
}