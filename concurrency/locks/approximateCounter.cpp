#include <vector>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <ctime>

class ApproxCounter {
public:
    std::vector<int> localCounters;
    std::mutex localMuxs[4];
    int globalCount;
    std::mutex globalMux;
    int size;
    int threshold;

    ApproxCounter(int size, int threshold) {
        this->size = size;
        this->threshold = threshold;
        this->localCounters.resize(size, 0);  // Initialize to 0
        this->globalCount = 0;
    }
    
    void increment(int idx) {
        int localValue;
        
        localMuxs[idx].lock();
        localCounters[idx]++;
        localValue = localCounters[idx];

        if (localValue >= threshold) {
            globalMux.lock();
    
            if (localCounters[idx] >= threshold) {
                globalCount += localCounters[idx];
                localCounters[idx] = 0;
            }
            globalMux.unlock();
        }
        localMuxs[idx].unlock();
    }
};

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

void bigIncrement(ApproxCounter* counter, int idx) {
    for (int i = 0; i < 1e7; i++) {
        counter->increment(idx);
    }
}

int main() {
    ApproxCounter counter(4, 100000);
    std::thread threads[4];
    
    printTime();
    
    for (int i = 0; i < 4; i++) {
        threads[i] = std::thread(bigIncrement, &counter, i);
    }
    
    for (int i = 0; i < 4; i++) {
        threads[i].join();
    }
    
    // Add remaining local counts to global count
    for (int i = 0; i < counter.size; i++) {
        counter.globalCount += counter.localCounters[i];
    }
    printTime();
    
    std::cout << "Final count: " << counter.globalCount << std::endl;
    return 0;
}