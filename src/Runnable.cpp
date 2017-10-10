
#include "Runnable.hpp"


void Runnable::join() {
    while (!isDone()) {}
    thread.join();
}

void Runnable::run(std::function<void()> task) {
    if (!done.load()) throw std::runtime_error {"Tried to start a running runnable"};

    done.store(false);
    thread = std::thread{[this, task = std::move(task)] {
        task();
        done.store(true);
        stopSignal.store(false);
    }};
}
