
#ifndef PIGEONSQUARE_TASKQUEUE_HPP
#define PIGEONSQUARE_TASKQUEUE_HPP

#include <functional>
#include <mutex>
#include <queue>

#include <SFML/System.hpp>
#include <cmath>


using lock_t = std::lock_guard<std::mutex>;

struct Bread {
    sf::Vector2f position {};
    int id {-1};
};

class TaskQueue {
public:
    template <class F>
    void enqueue(F&& task) {
        lock_t lock {tasksMutex};

        tasks.emplace(std::forward<F>(task));
    }

    void consume() {
        lock_t lock {tasksMutex};

        while (!tasks.empty()) {
            tasks.front()();
            tasks.pop();
        }
    }
private:
    std::queue<std::function<void()>> tasks;
    std::mutex tasksMutex;
};

inline float length(sf::Vector2f const& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}


#endif //PIGEONSQUARE_TASKQUEUE_HPP
