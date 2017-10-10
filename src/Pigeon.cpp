
#include <cmath>
#include "Pigeon.hpp"

#include "Window.hpp"
#include "World.hpp"


using namespace std::chrono;


void Pigeon::start(PigeonConfig const& pigeonConfig) {
    config = pigeonConfig;
    randomSeed = pigeonConfig.randomSeed++;
    run([this] {
        refreshTime = 1.f / config.refreshRate;
        config.pWorld->onPigeonStarted(*this);
        position = genPosition();
        spriteId = config.pWindow->addSprite("pigeon", position);

        while (!mustStop()) {
            auto timeBeginUpdate = high_resolution_clock::now();

            // Get target
            sf::Vector2f target;
            {
                lock_t lock {targetMutex};
                target = this->target;
            }

            // Move towards target
            sf::Vector2f diff = target - position;
            float diffLength = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (diffLength < refreshTime * config.speed) {
                setPosition(target);
                if (chasingBread) {
                    config.pWorld->onBreadEaten(*this, bread);
                }
            }
            else {
                sf::Vector2f shift = (diff / diffLength) * refreshTime * config.speed;
                move(shift);
            }

            // Respect refresh rate
            auto timeEndUpdate = high_resolution_clock::now() - timeBeginUpdate;
            std::this_thread::sleep_for(std::chrono::duration<float>(refreshTime) - timeEndUpdate);
        }
        config.pWindow->removeSprite(spriteId);
        config.pWorld->onPigeonStopped(*this);
    });
}

void Pigeon::onBreadEaten(Pigeon const &eater) {
    if (&eater == this) {
        ++score;
    }
    lock_t lock {targetMutex};

    target = genPosition();
    chasingBread = false;
}

void Pigeon::onBreadCreated(Bread const &bread) {
    lock_t lock {targetMutex};

    target = bread.position;
    this->bread = bread;
    chasingBread = true;
}

void Pigeon::onRockLaunched(sf::Vector2f position) {
    lock_t lock {targetMutex};

    target = genPosition();
    chasingBread = false;
}

void Pigeon::move(sf::Vector2f const &shift) {
    position += shift;
    config.pWindow->setSpritePosition(spriteId, position);
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
