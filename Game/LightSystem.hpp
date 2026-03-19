#pragma once

#include <SFML/Graphics.hpp>
#include "Settings.hpp"
#include <vector>

struct Light
{
    sf::Vector2f position;
    float radius;
};

struct Flashlight
{
    sf::Vector2f position;
    float angle;
    float radius;
    float distance;
    bool violet = false;
};

class LightSystem
{
private:

    std::vector<Light> lights;
    std::vector<Flashlight> flashlights;
    sf::RenderTexture lightMap;
    sf::Vector2u windowSize;

public:

    LightSystem(sf::Vector2u windowSize);

    void clearLights();
    void addLight(sf::Vector2f pos, float radius);
    void addFlashlight(sf::Vector2f pos, float angle, float radius, float distance, bool violet = false);

    void render(sf::RenderWindow& window, const GameSettings& settings);
};