#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Enemy.hpp"
#include "Platform.hpp"
#include "LightSystem.hpp"
#include "ParallaxBackground.hpp"
#include "Settings.hpp"

class Scene
{
private:

    Player player;
    Enemy enemy;
    ParallaxBackground background;

    std::vector<Platform> platforms;

    LightSystem lights;

    GameSettings settings;

public:

    Scene(sf::Vector2u windowSize, const GameSettings& settings);

    void update(float dt);
    void render(sf::RenderWindow& window);

    sf::Vector2f getPlayerPosition() const;
    sf::Vector2f getPlayerVelocity() const;
};