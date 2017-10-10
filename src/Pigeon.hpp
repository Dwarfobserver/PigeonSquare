
#ifndef PIGEONSQUARE_PIGEON_HPP
#define PIGEONSQUARE_PIGEON_HPP

#include <thread>

#include <SFML/System.hpp>
#include <atomic>
#include <mutex>
#include <queue>

#include "PigeonConfig.hpp"
#include "Runnable.hpp"
#include "Commons.hpp"


class Pigeon : public Runnable {
    static int nextId;
public:
    const int id;
    Pigeon() : id(++nextId) {}

    void start(PigeonConfig const& config);

    int getScore() const { return score.load(); }

    // Events from World
    void onBreadEaten(Pigeon const& eater);
    void onBreadCreated(Bread const& bread);
    void onRockLaunched(sf::Vector2f const& position);
private:
    PigeonConfig config;
    TaskQueue eventTasks;

    sf::Vector2f position;
    int spriteId;
    std::atomic_int score;

    void setPosition(sf::Vector2f const& pos);
    bool move(sf::Vector2f const &target);

    // Returns true if the state is finished
    std::function<bool()> state;
    bool isFleeing;

    mutable int randomSeed;

    int genRandom() const;
    int genRandom(int min, int max) const;
    sf::Vector2f genPosition() const;
};


#endif //PIGEONSQUARE_PIGEON_HPP
