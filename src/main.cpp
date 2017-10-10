
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Window.hpp"
#include "PigeonConfig.hpp"
#include "Pigeon.hpp"
#include "World.hpp"


int main() {
    Window window;
    World world;
    std::vector<Pigeon> pigeons(50);

    PigeonConfig config;
    config.pWindow = &window;
    config.pWorld = &world;
    config.area = {800, 600};
    config.speed = 100.f;
    config.randomSeed = 123478;
    config.refreshRate = 60;

    window.addTexture("pigeon", "images/pigeon.png");
    window.addTexture("bread", "images/bread.png");
    window.addTexture("bread expired", "images/bread expired.png");

    world.create(window);
    for (auto& p : pigeons) p.start(config);
    window.start(config.area, world);

    window.join();
    for (auto& p : pigeons) p.stop();
    for (auto& p : pigeons) p.join();

    return 0;
}
