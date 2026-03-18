#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Platform.hpp"

class Box
{
private:
    sf::RectangleShape shape; // collisions
    sf::Sprite sprite;        // rendu
    sf::Vector2f velocity = { 0.f, 0.f };
    bool isGrounded = false;

    float spriteOffset = 0.f;

public:
    Box(sf::Vector2f pos);
    void update(float dt, std::vector<Platform>& platforms,
        sf::FloatRect playerBounds, sf::Vector2f playerVel,
        sf::FloatRect plateBounds);
    void render(sf::RenderWindow& window);
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    sf::Vector2f getPosition() const { return shape.getPosition(); }
};