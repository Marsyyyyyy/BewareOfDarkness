#include "Scene.hpp"

Scene::Scene()
{
    platforms.push_back(Platform({ 0,650 }, { 1280,70 }));
    platforms.push_back(Platform({ 400,500 }, { 200,30 }));
}

void Scene::update(float dt)
{
    player.update(dt, platforms);
    enemy.update(dt, player.getPosition());
}

void Scene::render(sf::RenderWindow& window)
{
    for (auto& p : platforms)
        p.render(window);

    player.render(window);
    enemy.render(window);

    lights.render(window);
}