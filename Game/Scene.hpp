#pragma once

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Enemy.hpp"
#include "Platform.hpp"
#include "LightSystem.hpp"

class Scene
{
private:

    Player player;
    Enemy enemy;

    std::vector<Platform> platforms;

    LightSystem lights;

public:

    Scene();

    void update(float dt);
    void render(sf::RenderWindow& window);
};