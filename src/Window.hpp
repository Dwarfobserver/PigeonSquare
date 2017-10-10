
#ifndef PIGEONSQUARE_WINDOW_HPP
#define PIGEONSQUARE_WINDOW_HPP

#include <mutex>
#include <queue>
#include <functional>
#include <set>

#include <SFML/Graphics.hpp>

#include "Runnable.hpp"


class Window : public Runnable {
    using lock_t = std::lock_guard<std::mutex>;

    struct SpriteSorter {
        bool operator()(sf::Sprite* s1, sf::Sprite* s2) {
            return s1->getPosition().y < s2->getPosition().y;
        }
    };
public:
    void start(sf::Vector2u const& size);

    void addTexture(std::string const& name, std::string const& file);

    int addSprite(std::string const& textureName, sf::Vector2f const& pos);
    void setSpritePosition(int spriteId, sf::Vector2f const& pos);
    void removeSprite(int spriteId);
private:
    sf::RenderWindow window;

    std::map<std::string, sf::Texture> textures;

    std::map<int, sf::Sprite> sprites;
    int nextSpriteId;
    std::set<sf::Sprite*, SpriteSorter> sortedSprites;

    std::queue<std::function<void()>> spritesTasks;
    std::mutex spritesMutex;

    void update();
    void draw();
    void handleInputs();
};


#endif //PIGEONSQUARE_WINDOW_HPP
