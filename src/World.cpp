//
// Created on 09/10/2017.
//

#include "World.hpp"

#include "Pigeon.hpp"
#include "Window.hpp"



void World::create(WorldConfig const &config) {
    this->config = config;
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

void World::createBread(sf::Vector2f const &position) {
    config.pWindow->removeSprite(bread.id);
    if (validBread.load()) {
        config.pWindow->addSprite(config.breadExpiredTextureName, bread.position);
    }
    bread.position = position;
    bread.id = config.pWindow->addSprite(config.breadTextureName, position);

    // Trigger all
}

void World::eatBread(Pigeon &pigeon, Bread const& bread) {
    lock_t lock {pigeonsMutex};

    if (!validBread.load() || bread.id != this->bread.id) return;

    if (bread.id == this->bread.id) {
        pigeon.onBreadEaten();

        // Trigger all
    }
}
