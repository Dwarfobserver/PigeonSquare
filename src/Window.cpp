
#include "Window.hpp"


void Window::start(sf::Vector2u const& size) {
    run([this, size] {
        window.create({size.x, size.y}, "Pigeon Square - C++14 & SFML");
        window.setVerticalSyncEnabled(true);

        while (!mustStop())
        {
            update();
            draw();
            handleInputs();
        }
    });
}

void Window::addTexture(std::string const &name, std::string const &file) {
    lock_t lock {spritesMutex};

    spritesTasks.emplace([this, name, file] {
        textures[name].loadFromFile("../../" + file);
    });
}

int Window::addSprite(std::string const &textureName, sf::Vector2f const &pos) {
    lock_t lock {spritesMutex};

    int id = nextSpriteId++;
    spritesTasks.emplace([this, id, textureName, pos] {
        auto& sprite = sprites[id];

        auto const& texture = textures.at(textureName);
        sprite.setTexture(texture);
        sprite.setPosition(pos);
        sortedSprites.insert(&sprite);
    });
    return id;
}

void Window::setSpritePosition(int spriteId, sf::Vector2f const &pos) {
    lock_t lock {spritesMutex};

    spritesTasks.emplace([this, id = spriteId, pos] {
        auto& sprite = sprites[id];

        sortedSprites.erase(&sprite);
        sprite.setPosition(pos);
        sortedSprites.insert(&sprite);
    });
}

void Window::removeSprite(int spriteId) {
    lock_t lock {spritesMutex};

    spritesTasks.emplace([this, id = spriteId] {
        sortedSprites.erase(&sprites[id]);
        sprites.erase(id);
    });
}


void Window::update() {
    lock_t lock {spritesMutex};

    while (!spritesTasks.empty()) {
        spritesTasks.front()();
        spritesTasks.pop();
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
            stop();
        }
    }
}
