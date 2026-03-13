#include "Scene.hpp"
#include <iostream>

Scene::Scene(sf::Vector2u windowSize, const GameSettings& settings,
    std::function<void()> victoryCallback,
    std::function<void()> gameOverCallback)
    : settings(settings)
    , lights(windowSize)
    , background(windowSize)
    , box({ 1700.f, 880.f })
    , door({ 2500.f, 380.f }, { 100.f, 600.f })
    , plate({ 2200.f, 975.f })
    , victoryZone({ 3850.f, 700.f }, { 100.f, 290.f })
    , onVictory(victoryCallback)
    , onGameOver(gameOverCallback)
{
    platforms.push_back(Platform({ 0.f,    980.f }, { 700.f,  100.f }));
    platforms.push_back(Platform({ 950.f,  980.f }, { 3050.f, 100.f }));
    platforms.push_back(Platform({ 1200.f, 880.f }, { 120.f,  100.f }));
    platforms.push_back(Platform({ 3500.f, 700.f }, { 60.f,   380.f }));
}

void Scene::update(float dt)
{
    // Solides de base (sans la box)
    std::vector<Platform> allSolids = platforms;
    if (!door.isOpen())
        allSolids.push_back(Platform({ 2500.f, 380.f }, { 100.f, 600.f }));

    // Update joueur sans la box
    player.update(dt, allSolids, settings);

    // Update box
    box.update(dt, allSolids, player.getBounds(), player.getVelocity());

    // === Collision manuelle joueur <-> box ===
    auto inter = player.getBounds().findIntersection(box.getBounds());
    if (inter)
    {
        float playerCenterY = player.getBounds().position.y + player.getBounds().size.y / 2.f;
        float boxCenterY = box.getBounds().position.y + box.getBounds().size.y / 2.f;
        float playerCenterX = player.getBounds().position.x + player.getBounds().size.x / 2.f;
        float boxCenterX = box.getBounds().position.x + box.getBounds().size.x / 2.f;

        // Déterminer si collision par le dessus ou par le côté
        if (playerCenterY < boxCenterY && inter->size.y < inter->size.x)
        {
            // Joueur sur le dessus de la box -> le poser dessus
            player.setPositionY(box.getBounds().position.y - player.getBounds().size.y);
            player.setGrounded();
        }
        else
        {
            // Collision latérale -> repousser le joueur
            if (playerCenterX < boxCenterX)
                player.moveX(-inter->size.x);
            else
                player.moveX(inter->size.x);
        }
    }

    plate.update(box.getBounds());
    door.update(plate.isActivated(), dt);
    background.update(player.getVelocity().x, dt);

    if (!victoryTriggered && player.getPosition().y > 1200.f)
        onGameOver();

    if (!victoryTriggered && victoryZone.checkPlayer(player.getBounds()))
    {
        victoryTriggered = true;
        onVictory();
    }
}

void Scene::render(sf::RenderWindow& window)
{
    background.render(window);
}

void Scene::renderWorld(sf::RenderWindow& window)
{
    for (auto& p : platforms) p.render(window);
    enemy.render(window);
    plate.render(window);
    door.render(window);
    box.render(window);
    victoryZone.render(window);
    player.render(window);
    
    if (settings.lightsEnabled)
    {
        lights.clearLights();
        lights.addLight(player.getPosition() + sf::Vector2f(0.f, -20.f), 200.f);
        lights.addFlashlight(player.getPosition() + sf::Vector2f(0.f, -20.f), player.getAimAngle(), 120.f, 500.f);
        lights.render(window, settings);
    }
}

sf::Vector2f Scene::getPlayerPosition() const { return player.getPosition(); }
sf::Vector2f Scene::getPlayerVelocity() const { return player.getVelocity(); }