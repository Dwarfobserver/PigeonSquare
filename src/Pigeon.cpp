
#include "Pigeon.hpp"

#include "Window.hpp"
#include "World.hpp"


using namespace std::chrono;

Pigeon::Pigeon(PigeonConfig const &config) :
        config(config)
{
    randomSeed = config.randomSeed++;

    position = genPosition();

    refreshTime = 1.f / config.refreshRate;
}

void Pigeon::start() {
    run([this] {
        config.pWorld->onPigeonStarted(*this);
        spriteId = config.pWindow->addSprite(config.textureName, position);

        while (!mustStop()) {
            auto timeBeginUpdate = high_resolution_clock::now();


            // Respect refresh rate
            auto timeEndUpdate = high_resolution_clock::now() - timeBeginUpdate;
            std::this_thread::sleep_for(std::chrono::duration<float>(refreshTime) - timeEndUpdate);
        }
        config.pWindow->removeSprite(spriteId);
        config.pWorld->onPigeonStopped(*this);
    });
}

int Pigeon::getScore() const {
    return score.load();
}

void Pigeon::onBreadEaten() {
    ++score;
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
