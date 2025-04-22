#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

const int threads=4;
const int cif=100000000;
std::mutex mtx;
int mutex_counter=0;
std::atomic<int> atomic_counter;

void mutex_inc(){
    for (int i=0;i<cif;i++){
        mtx.lock();
        mutex_counter++;
        mtx.unlock();
    }
}

void atomic_inc(){
    for (int i=0;i<cif;i++){
        atomic_counter++;
    }
}

int main(){
    //mutex
    auto start_mtx=std::chrono::high_resolution_clock::now();
    std::vector <std::thread> mtx_threads;
    for (int i=0;i<threads;i++){
        std::thread a(mutex_inc);
        mtx_threads.push_back(std::move(a));
    }

    for (auto& thread : mtx_threads) {
        thread.join();
    }
    
    auto end_mtx=std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> diff_mtx=end_mtx-start_mtx;
    std::cout<<"mutex_time:"<<diff_mtx.count()<<std::endl;
    std::cout<<"mutex_counter:"<<mutex_counter<<std::endl;

    //atomic
    auto start_atomic=std::chrono::high_resolution_clock::now();
    std::vector <std::thread> at_threads;
    for (int i=0;i<threads;i++){
        std::thread b(atomic_inc);
        at_threads.push_back(std::move(b));
    }
    for (auto& thread : at_threads) {
        thread.join();
    }

    auto end_atomic=std::chrono::high_resolution_clock::now();
    std::chrono::duration <double> diff_atomic=end_atomic-start_atomic;
    std::cout<<"atomic_time:"<<diff_atomic.count()<<std::endl;
    std::cout<<"atomic_counter:"<<atomic_counter<<std::endl;

}