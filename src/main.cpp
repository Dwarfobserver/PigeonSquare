
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Window.hpp"
#include "PigeonConfig.hpp"
#include "Pigeon.hpp"
#include "World.hpp"


int main() {
    Window window; // Store textures, maintain and draw sprites, receive inputs
    World world;   // Dispatch events to pigeons

    std::vector<Pigeon> pigeons(20);

    PigeonConfig config;
    config.pWindow = &window;
    config.pWorld = &world;
    config.area = {1400, 800};       // Screen size, where pigeons navigate
    config.sleepDelay = 2.f;        // Delay between bread
    config.speed = 150.f;            // In pixels per second
    config.fleeRadius = 250.f;       // Range under which pigeons react to right click
    config.fleeRange = 400.f;        // Maximum range near right click within pigeons flee
    config.randomSeed = 456'789'123; // Incremented at each pigeon start for convenience
    config.refreshRate = 60.f;       // Number of pigeons update by second

    window.addTexture("pigeon", "images/pigeon.png");
    window.addTexture("bread", "images/bread.png");
    window.addTexture("bread expired", "images/bread expired.png");

    world.create(window);
    for (auto& p : pigeons) p.start(config);

    // Blocking call (window must be executed in main thread in OSX)
    window.run(world, config.area);

    // Stop application when the window stop running
    for (auto& p : pigeons) p.stop();
    for (auto& p : pigeons) p.join();

    return 0;
}
