#pragma once

#include "Entity.hpp"
#include "Platform.hpp"
#include "Settings.hpp"
#include <vector>

class Player : public Entity
{
private:

    bool isGrounded;
    sf::Sprite sprite;
    sf::Texture walkTexture;
    sf::Texture upTexture;
    bool facingRight = true;

    int currentFrame = 0;
    float animTimer = 0.f;
    float animFrameTime = 0.12f;
    bool lookingUp = false;

public:

    Player();

    void update(float dt, std::vector<Platform>& platforms, const GameSettings& settings);
    void render(sf::RenderWindow& window);
    void setPositionY(float y);
    void setGrounded();
    void moveX(float dx);

    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    sf::FloatRect getBounds() const;
    float getAimAngle() const;

private:
    float lastAimAngle;
};