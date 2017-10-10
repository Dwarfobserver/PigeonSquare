
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Window.hpp"
#include "PigeonConfig.hpp"
#include "Pigeon.hpp"
#include "World.hpp"


int main() {
    Window window;
    World world;

    WorldConfig worldConfig;

    worldConfig.pWindow = &window;
    worldConfig.breadTextureName = "bread";
    worldConfig.breadExpiredTextureName = "bread expired";

    PigeonConfig pigeonConfig;

    pigeonConfig.pWindow = &window;
    pigeonConfig.pWorld = &world;
    pigeonConfig.area = {800, 600};
    pigeonConfig.textureName = "pigeon";
    pigeonConfig.speed = 100.f;
    pigeonConfig.randomSeed = 123478;
    pigeonConfig.refreshRate = 60;

    window.addTexture(pigeonConfig.textureName, "images/pigeon.png");
    window.addTexture(worldConfig.breadTextureName, "images/bread.png");
    window.addTexture(worldConfig.breadExpiredTextureName, "images/bread expired.png");

    Pigeon p1(pigeonConfig);

    window.start(pigeonConfig.area);
    while (window.isDone()) {}

    p1.start();

    window.addSprite("bread", {450, 120});
    
    window.join();

    p1.stop();
    p1.join();

    return 0;
}
