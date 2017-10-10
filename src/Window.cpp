
#include "Window.hpp"
#include "World.hpp"


void Window::run(World& world, sf::Vector2u const& size) {
    pWorld = &world;
    window.create({size.x, size.y}, "Pigeon Square - C++14 & SFML");
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        spriteTasks.consume();
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
        setSpritePosition(sprite, pos);
        sortedSprites.insert(&sprite);
    });
    return id;
}

void Window::setSpritePosition(int spriteId, sf::Vector2f const &pos) {

    spriteTasks.enqueue([this, id = spriteId, pos] {
        auto& sprite = sprites[id];

        eraseInMultiset(&sprite);
        setSpritePosition(sprite, pos);
        sortedSprites.insert(&sprite);
    });
}

void Window::removeSprite(int spriteId) {

    spriteTasks.enqueue([this, id = spriteId] {
        eraseInMultiset(&sprites[id]);
        sprites.erase(id);
    });
}

void Window::setSpritePosition(sf::Sprite &sprite, sf::Vector2f const &pos) {
    auto size = sf::Vector2f {
            static_cast<float>(sprite.getTextureRect().width),
            static_cast<float>(sprite.getTextureRect().height)
    };
    sprite.setPosition(pos - size / 2.f);
}

void Window::eraseInMultiset(sf::Sprite *pSprite) {

    auto it = sortedSprites.find(pSprite);
    while (it != sortedSprites.end()) {
        if (*it == pSprite) {
            auto it_copy = it;
            ++it_copy;
            sortedSprites.erase(it, it_copy);
            it = sortedSprites.end();
        }
        else ++it;
    }
}

void Window::draw() {
    window.clear(sf::Color::Black);

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
                pWorld->createBread(pos);
            }
            else if (event.mouseButton.button == sf::Mouse::Button::Right) {
                pWorld->launchRock(pos);
            }
        }
    }
}
