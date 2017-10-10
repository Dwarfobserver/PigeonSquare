
#ifndef PIGEONSQUARE_WORLD_HPP
#define PIGEONSQUARE_WORLD_HPP

#include <vector>
#include <mutex>
#include <set>
#include <atomic>

#include "Bread.hpp"
#include "WorldConfig.hpp"


class Pigeon;
class Window;

class World {
    using lock_t = std::lock_guard<std::mutex>;
public:
    void create(Window& window);

    void onPigeonStarted(Pigeon& pigeon);
    void onPigeonStopped(Pigeon& pigeon);
    void onBreadEaten(Pigeon &pigeon, Bread const &bread);

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
