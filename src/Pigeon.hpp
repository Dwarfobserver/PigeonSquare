
#ifndef PIGEONSQUARE_PIGEON_HPP
#define PIGEONSQUARE_PIGEON_HPP

#include <thread>

#include <SFML/System.hpp>
#include <atomic>

#include "PigeonConfig.hpp"
#include "Runnable.hpp"


class Pigeon : public Runnable {
public:
    explicit Pigeon(PigeonConfig const& config);

    void start();
    int getScore() const;
    void onBreadEaten();

    const PigeonConfig config;
private:
    float refreshTime;

    sf::Vector2f position;
    int spriteId;

    std::atomic_int score;

    mutable int randomSeed;

    int genRandom() const;
    int genRandom(int min, int max) const;
    sf::Vector2f genPosition() const;
};


#endif //PIGEONSQUARE_PIGEON_HPP
