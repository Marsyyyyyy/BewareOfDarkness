#pragma once
#include <SFML/Graphics.hpp>

class Door
{
private:
    sf::RectangleShape shape;
    sf::Sprite sprite;
    bool open = false;
    float currentHeight;
    float targetHeight;
    float fullHeight;
    sf::Vector2f basePos;

public:
    Door(sf::Vector2f pos, sf::Vector2f size);
    void update(bool activated, float dt);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isOpen() const { return open; }
};