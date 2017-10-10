
#ifndef PIGEONSQUARE_PIGEONCONFIG_HPP
#define PIGEONSQUARE_PIGEONCONFIG_HPP

#include <string>

#include <SFML/Graphics/Rect.hpp>


class Window;
class World;

struct PigeonConfig {
    Window* pWindow;
    World* pWorld;
    std::string textureName;
    sf::Vector2u area;
    float speed;
    int refreshRate;
    mutable int randomSeed{123456}; // Changed by pigeon ctor for convenience
};


#endif //PIGEONSQUARE_PIGEONCONFIG_HPP
