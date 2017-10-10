
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
    float sleepDelay;
    float speed;
    float fleeRadius;
    float fleeRange;
    float refreshRate;
    mutable int randomSeed;
};


#endif //PIGEONSQUARE_PIGEONCONFIG_HPP
