
#include "World.hpp"

#include "Pigeon.hpp"
#include "Window.hpp"


void World::create(Window& window) {
    pWindow = &window;
    validBread.store(false);
}

void World::onPigeonStarted(Pigeon &pigeon) {
    lock_t lock {pigeonsMutex};

    pigeons.insert(&pigeon);
}

void World::onPigeonStopped(Pigeon &pigeon) {
    lock_t lock {pigeonsMutex};

    pigeons.erase(&pigeon);
}

void World::onBreadEaten(Pigeon &pigeon, Bread const &bread) {
    lock_t lock {pigeonsMutex};

    if (!validBread.load() || bread.id != this->bread.id) return;

    std::cout << "pigeon " << pigeon.id << " ate ";
    if (pigeon.getScore() == 0) {
        std::cout << "his first bread\n";
    }
    else {
        std::cout << pigeon.getScore() + 1 << " breads\n";
    }

    validBread.store(false);
    pWindow->removeSprite(bread.id);
    for (auto pPigeon : pigeons) {
        pPigeon->onBreadEaten(pigeon);
    }
}

void World::createBread(sf::Vector2f const &position) {
    lock_t lock {pigeonsMutex};

    if (validBread.load()) {
        pWindow->removeSprite(bread.id);
        pWindow->addSprite("bread expired", bread.position);
    }
    bread.position = position;
    bread.id = pWindow->addSprite("bread", position);

    validBread.store(true);
    for (auto pPigeon : pigeons) {
        pPigeon->onBreadCreated(bread);
    }
}

void World::launchRock(sf::Vector2f const &position) {
    lock_t lock {pigeonsMutex};

    for (auto pPigeon : pigeons) {
        pPigeon->onRockLaunched(position);
    }
}
