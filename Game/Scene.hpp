#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Enemy.hpp"
#include "Platform.hpp"
#include "LightSystem.hpp"
#include "ParallaxBackground.hpp"

class Scene
{
private:

    Player player;
    Enemy enemy;
    ParallaxBackground background;

    std::vector<Platform> platforms;

    LightSystem lights;

public:

    Scene(sf::Vector2u windowSize);

    void update(float dt);
    void render(sf::RenderWindow& window);
};