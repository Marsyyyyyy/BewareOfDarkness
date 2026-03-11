#pragma once
#include <SFML/Graphics.hpp>

class Camera2D
{
private:

    sf::View view;

    sf::FloatRect levelBounds;
    sf::FloatRect deadZone;

    sf::Vector2f lookAhead;
    float smoothSpeed;

public:

    Camera2D(sf::Vector2u windowSize);

    void setLevelBounds(const sf::FloatRect& bounds);

    void update(float dt, const sf::Vector2f& playerPos, float playerVelocityX);

    void apply(sf::RenderWindow& window);

    sf::View& getView();
};