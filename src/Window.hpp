
#ifndef PIGEONSQUARE_WINDOW_HPP
#define PIGEONSQUARE_WINDOW_HPP

#include <mutex>
#include <queue>
#include <functional>
#include <set>

#include <SFML/Graphics.hpp>

#include "Commons.hpp"
#include "PigeonConfig.hpp"


class World;

class Window {
    struct SpriteSorter {
        bool operator()(sf::Sprite* s1, sf::Sprite* s2) {
            return s1->getPosition().y < s2->getPosition().y;
        }
    };
public:
    void run(World& world, sf::Vector2u const& size);

    void addTexture(std::string const& name, std::string const& file);

    int addSprite(std::string const& textureName, sf::Vector2f const& pos);
    void removeSprite(int spriteId);

    void setSpritePosition(int spriteId, sf::Vector2f const& pos);
private:
    PigeonConfig config;
    sf::RenderWindow window;
    World* pWorld;

    std::map<std::string, sf::Texture> textures;

    TaskQueue spriteTasks;

    std::map<int, sf::Sprite> sprites;
    int nextSpriteId;
    std::vector<sf::Sprite*> sortedSprites;

    void setSpritePosition(sf::Sprite& sprite, sf::Vector2f const& pos);

    void draw();
    void handleInputs();
};


#endif //PIGEONSQUARE_WINDOW_HPP
