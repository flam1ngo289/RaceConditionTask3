#include <chrono>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <Windows.h>

class Data {
public:
    int value;
    std::mutex mtx;

    Data(int val) : value(val) {}
};


void swap_using_lock(Data& lhs, Data& rhs) {
    std::lock(lhs.mtx, rhs.mtx);

    std::lock_guard<std::mutex> lock_lhs(lhs.mtx, std::adopt_lock);
    std::lock_guard<std::mutex> lock_rhs(rhs.mtx, std::adopt_lock);

    std::swap(lhs.value, rhs.value);
}


void swap_using_scoped_lock(Data& lhs, Data& rhs) {
    std::scoped_lock lock(lhs.mtx, rhs.mtx);

    std::swap(lhs.value, rhs.value);
}


void swap_using_unique_lock(Data& lhs, Data& rhs) {
    std::unique_lock<std::mutex> lock_lhs(lhs.mtx, std::defer_lock);
    std::unique_lock<std::mutex> lock_rhs(rhs.mtx, std::defer_lock);

    std::lock(lock_lhs, lock_rhs);

    std::swap(lhs.value, rhs.value);
}

int main() {
    setlocale(LC_ALL, "RU");
    Data a(10);
    Data b(20);

    std::cout << "Исходные значения: a = " << a.value << ", b = " << b.value << "\n";

 
    std::thread t1(swap_using_lock, std::ref(a), std::ref(b));
    t1.join();
    std::cout << "После swap_using_lock: a = " << a.value << ", b = " << b.value << "\n";

    t1 = std::thread(swap_using_lock, std::ref(a), std::ref(b));
    t1.join();

    std::thread t2(swap_using_scoped_lock, std::ref(a), std::ref(b));
    t2.join();
    std::cout << "После swap_using_scoped_lock: a = " << a.value << ", b = " << b.value << "\n";

    
    t2 = std::thread(swap_using_scoped_lock, std::ref(a), std::ref(b));
    t2.join();


    std::thread t3(swap_using_unique_lock, std::ref(a), std::ref(b));
    t3.join();
    std::cout << "После swap_using_unique_lock: a = " << a.value << ", b = " << b.value << "\n";

    return 0;
}
