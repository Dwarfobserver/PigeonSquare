
#include "Window.hpp"
#include "World.hpp"


void Window::run(PigeonConfig const& config) {
    this->config = config;

    window.create({config.area.x, config.area.y}, "Pigeon Square - C++14 & SFML");
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        update();
        draw();
        handleInputs();
    }
}

void Window::addTexture(std::string const &name, std::string const &file) {

    spriteTasks.enqueue([this, name, file] {
        textures[name].loadFromFile("../../" + file);
    });
}

int Window::addSprite(std::string const &textureName, sf::Vector2f const &pos) {
    int id = nextSpriteId++;

    spriteTasks.enqueue([this, id, textureName, pos] {
        auto& sprite = sprites[id];

        auto const& texture = textures.at(textureName);
        sprite.setTexture(texture);
        setPosition(sprite, pos);
        sortedSprites.push_back(&sprite);

        spritePositions.emplace(id, pos);
    });
    return id;
}

void Window::setSpritePosition(int spriteId, sf::Vector2f const &pos) {
    auto& info = spritePositions[spriteId];
    info.pos[info.lastPos.fetch_xor(1, std::memory_order_relaxed)] = pos;
}

void Window::removeSprite(int spriteId) {

    spriteTasks.enqueue([this, id = spriteId] {
        auto it = std::find(sortedSprites.begin(), sortedSprites.end(), &sprites[id]);
        sortedSprites.erase(it);
        sprites.erase(id);
        spritePositions.erase(id);
    });
}

void Window::setPosition(sf::Sprite &sprite, sf::Vector2f const &pos) {
    auto ajustedPos = sf::Vector2f {
            pos.x - sprite.getTextureRect().width / 2.f,
            pos.y - sprite.getTextureRect().height / 2.f
    };
    sprite.setPosition(ajustedPos);
}

void Window::update() {
    spriteTasks.consume();

    for (auto& it : spritePositions) {
        int i = it.second.lastPos.load(std::memory_order_relaxed);
        setPosition(sprites[it.first], it.second.pos[i]);
    }

    std::stable_sort(sortedSprites.begin(), sortedSprites.end(), [] (sf::Sprite* s1, sf::Sprite* s2) {
        auto y1 = s1->getPosition().y + s1->getTextureRect().height;
        auto y2 = s2->getPosition().y + s2->getTextureRect().height;
        return y1 < y2;
    });
}

void Window::draw() {
    window.clear(sf::Color::White);

    for (auto pSprite : sortedSprites) {
        window.draw(*pSprite);
    }

    window.display();
}

void Window::handleInputs() {
    sf::Event event{};
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            sf::Vector2f pos = {
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
            };
            if (event.mouseButton.button == sf::Mouse::Button::Left) {
                config.pWorld->createBread(pos);
            }
            else if (event.mouseButton.button == sf::Mouse::Button::Right) {
                config.pWorld->launchRock(pos);
            }
        }
    }
}
