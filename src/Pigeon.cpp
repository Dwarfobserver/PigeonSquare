
#include <cmath>
#include "Pigeon.hpp"

#include "Window.hpp"
#include "World.hpp"


using namespace std::chrono;

int Pigeon::nextId{0};

void Pigeon::start(PigeonConfig const& pigeonConfig) {
    config = pigeonConfig;
    randomSeed = pigeonConfig.randomSeed++;
    run([this] {
        config.pWorld->onPigeonStarted(*this);
        position = genPosition();
        spriteId = config.pWindow->addSprite("pigeon", position);

        state = [] { return true; };

        while (!mustStop()) {
            auto timeBeginUpdate = high_resolution_clock::now();

            if (state()) state = [] { return true; };
            eventTasks.consume();

            // Respect refresh rate
            auto timeEndUpdate = high_resolution_clock::now() - timeBeginUpdate;
            std::this_thread::sleep_for(duration<float>(1.f / config.refreshRate) - timeEndUpdate);
        }
        config.pWindow->removeSprite(spriteId);
        config.pWorld->onPigeonStopped(*this);
    });
}

void Pigeon::onBreadEaten(Pigeon const &eater) {

    eventTasks.enqueue([this, pEater = &eater] {
        if (this == pEater) { ++score; }

        auto target = genPosition();
        auto beginTime = high_resolution_clock::now();

        state = [this, target, beginTime] {
            // Wait
            if (high_resolution_clock::now() - beginTime < duration<float>(config.sleepDelay)) {
                return false;
            }
            // Go to sleep
            return move(target);
        };
    });
}

void Pigeon::onBreadCreated(Bread const &bread) {

    eventTasks.enqueue([this, bread] {

        state = [this, bread] {
            // Eat bread
            if (move(bread.position)) {
                config.pWorld->onBreadEaten(*this, bread);
                return true;
            }
            // Go to bread
            else return false;
        };
    });
}

void Pigeon::onRockLaunched(sf::Vector2f const& position) {

    eventTasks.enqueue([this, position] {

        auto diffLength = length(this->position - position);
        if (diffLength < config.fleeRadius) {

            auto target = genPosition();

            state = [this, target] {
                // Flee to target
                return move(target);
            };
        }
    });
}

bool Pigeon::move(sf::Vector2f const &target) {
    sf::Vector2f diff = target - position;
    float diffLength = length(diff);

    if (diffLength < (1.f / config.refreshRate) * config.speed) {
        setPosition(target);
        return true;
    }
    else {
        sf::Vector2f shift = (diff / diffLength) * (1.f / config.refreshRate) * config.speed;
        setPosition(position + shift);
        return false;
    }
}

void Pigeon::setPosition(sf::Vector2f const &pos) {
    position = pos;
    config.pWindow->setSpritePosition(spriteId, position);
}

int Pigeon::genRandom() const {
    int x = randomSeed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    randomSeed = x;
    return x;
}

int Pigeon::genRandom(int min, int max) const {
    return (std::abs(genRandom()) % (max - min)) + min;
}

sf::Vector2f Pigeon::genPosition() const {
    return sf::Vector2f{
            static_cast<float>(genRandom(0, config.area.x)),
            static_cast<float>(genRandom(0, config.area.y))
    };
}
