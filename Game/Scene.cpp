#include "Scene.hpp"

Scene::Scene(sf::Vector2u windowSize) : background(windowSize)
{
    platforms.push_back(Platform({ 0.f, 980.f }, { 1920.f, 100.f }));
    platforms.push_back(Platform({ 400.f, 800.f }, { 200.f, 30.f }));
}

void Scene::update(float dt)
{
    player.update(dt, platforms);
    enemy.update(dt, player.getPosition());
    background.update(player.getVelocity().x, dt);
}

void Scene::render(sf::RenderWindow& window)
{
    background.render(window);
    for (auto& p : platforms)
        p.render(window);
    player.render(window);
    enemy.render(window);
    lights.render(window);
}