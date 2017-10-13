
#ifndef PIGEONSQUARE_WINDOW_HPP
#define PIGEONSQUARE_WINDOW_HPP

#include <atomic>
#include <queue>
#include <functional>
#include <unordered_map>
#include <set>

#include <SFML/Graphics.hpp>

#include "Commons.hpp"
#include "PigeonConfig.hpp"


class World;

class Window {
    struct SpriteInfo {
        SpriteInfo() {}
        SpriteInfo(sf::Vector2f const& pos) : lastPos{0}, pos{pos, pos} {}

        // Rocker device to write at the safe place
        std::atomic_int lastPos;
        sf::Vector2f pos[2];
    };
public:
    void run(PigeonConfig const& config);

    void addTexture(std::string const& name, std::string const& file);

    int addSprite(std::string const& textureName, sf::Vector2f const& pos);
    void removeSprite(int spriteId);

    void setSpritePosition(int spriteId, sf::Vector2f const& pos);
private:
    PigeonConfig config;
    sf::RenderWindow window;

    std::unordered_map<std::string, sf::Texture> textures;

    std::map<int, SpriteInfo> spritePositions;

    TaskQueue spriteTasks;
    std::unordered_map<int, sf::Sprite> sprites;
    std::atomic_int nextSpriteId;
    std::vector<sf::Sprite*> sortedSprites;

    void setPosition(sf::Sprite &sprite, sf::Vector2f const &pos);

    void update();
    void draw();
    void handleInputs();
};


#endif //PIGEONSQUARE_WINDOW_HPP
