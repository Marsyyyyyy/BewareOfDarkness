#include "Scene.hpp"

Scene::Scene(sf::Vector2u windowSize, const GameSettings& settings)
    : settings(settings), background(windowSize), lights(windowSize)
{
    platforms.push_back(Platform({ 0.f, 980.f }, { 1920.f, 100.f }));
    platforms.push_back(Platform({ 400.f, 800.f }, { 200.f, 30.f }));
}

void Scene::update(float dt)
{
    player.update(dt, platforms, settings);
    enemy.update(dt, player.getPosition());
    background.update(player.getVelocity().x, dt);
}

void Scene::render(sf::RenderWindow& window)
{
    background.render(window);
    for (auto& p : platforms)
        p.render(window);
    enemy.render(window);
    player.render(window);

    lights.clearLights();
    lights.addLight(player.getPosition() + sf::Vector2f(0.f, -20.f), 200.f);
    lights.addFlashlight(player.getPosition() + sf::Vector2f(0.f, -20.f), player.getAimAngle(), 120.f, 500.f);
    lights.render(window, settings);
}

sf::Vector2f Scene::getPlayerPosition() const
{
    return player.getPosition();
}

sf::Vector2f Scene::getPlayerVelocity() const
{
    return player.getVelocity();
}
