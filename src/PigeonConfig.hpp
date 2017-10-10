
#ifndef PIGEONSQUARE_PIGEONCONFIG_HPP
#define PIGEONSQUARE_PIGEONCONFIG_HPP

#include <string>

#include <SFML/Graphics/Rect.hpp>


class Window;
class World;

struct PigeonConfig {
    Window* pWindow;
    World* pWorld;
    sf::Vector2u area;
    float speed;
    int refreshRate;
    mutable int randomSeed; // Changed by pigeon ctor for convenience
};


#endif //PIGEONSQUARE_PIGEONCONFIG_HPP
