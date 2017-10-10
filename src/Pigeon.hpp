
#ifndef PIGEONSQUARE_PIGEON_HPP
#define PIGEONSQUARE_PIGEON_HPP

#include <thread>

#include <SFML/System.hpp>
#include <atomic>
#include <mutex>

#include "PigeonConfig.hpp"
#include "Runnable.hpp"
#include "Bread.hpp"


class Pigeon : public Runnable {
    using lock_t = std::lock_guard<std::mutex>;
public:
    void start(PigeonConfig const& config);

    int getScore() const { return score.load(); }

    void onBreadEaten(Pigeon const& eater);
    void onBreadCreated(Bread const& bread);
    void onRockLaunched(sf::Vector2f position);
private:
    PigeonConfig config;
    float refreshTime;

    sf::Vector2f position;
    int spriteId;
    std::atomic_int score;

    sf::Vector2f target;
    Bread bread;
    bool chasingBread;
    std::mutex targetMutex;

    void move(sf::Vector2f const& shift);
    void setPosition(sf::Vector2f const& pos);

    mutable int randomSeed;

    int genRandom() const;
    int genRandom(int min, int max) const;
    sf::Vector2f genPosition() const;
};


#endif //PIGEONSQUARE_PIGEON_HPP
