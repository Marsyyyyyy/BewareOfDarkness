#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

struct Light
{
    sf::Vector2f position;
    float radius;
};

class LightSystem
{
private:

    std::vector<Light> lights;

public:

    LightSystem();

    void addLight(sf::Vector2f pos, float radius);

    void render(sf::RenderWindow& window);
};