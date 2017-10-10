
#ifndef PIGEONSQUARE_RUNNABLE_HPP
#define PIGEONSQUARE_RUNNABLE_HPP

#include <thread>
#include <atomic>
#include <functional>
#include <iostream>


class Runnable {
public:
    void stop() { stopSignal.store(true); }
    bool mustStop() const { return stopSignal.load(); }
    bool isDone() const { return done.load(); }
    void join() { while (!isDone()) {} thread.join(); }

    Runnable() : done(true), stopSignal(false) {}
    virtual ~Runnable() = default;
protected:
    void run(std::function<void()> task);
private:
    std::thread thread;
    std::atomic_bool done;
    std::atomic_bool stopSignal;
};


#endif //PIGEONSQUARE_RUNNABLE_HPP
