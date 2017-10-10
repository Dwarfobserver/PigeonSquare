
#ifndef PIGEONSQUARE_WORLD_HPP
#define PIGEONSQUARE_WORLD_HPP

#include <vector>
#include <mutex>
#include <set>
#include <atomic>

#include <SFML/System.hpp>

#include "Commons.hpp"


class Pigeon;
class Window;

class World {
public:
    void create(Window& window);

    // Events from pigeons
    void onPigeonStarted(Pigeon& pigeon);
    void onPigeonStopped(Pigeon& pigeon);
    void onBreadEaten(Pigeon &pigeon, Bread const &bread);

    // Inputs actions
    void createBread(sf::Vector2f const& position);
    void launchRock(sf::Vector2f const& position);
private:
    Window* pWindow;

    std::set<Pigeon*> pigeons;
    mutable std::mutex pigeonsMutex;

    Bread bread;
    std::atomic_bool validBread;
};


#endif //PIGEONSQUARE_WORLD_HPP
