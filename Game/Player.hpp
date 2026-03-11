#pragma once

#include "Entity.hpp"
#include "Platform.hpp"
#include <vector>

class Player : public Entity
{
private:

    bool isGrounded;

public:

    Player();

    void update(float dt, std::vector<Platform>& platforms);
    void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
};