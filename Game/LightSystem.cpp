#include "LightSystem.hpp"

LightSystem::LightSystem()
{
}

void LightSystem::addLight(sf::Vector2f pos, float radius)
{
    lights.push_back({ pos,radius });
}

void LightSystem::render(sf::RenderWindow& window)
{
    for (auto& l : lights)
    {
        sf::CircleShape light(l.radius);
        light.setOrigin({ l.radius,l.radius });
        light.setPosition(l.position);
        light.setFillColor(sf::Color(255, 255, 255, 40));

        window.draw(light, sf::BlendAdd);
    }
}