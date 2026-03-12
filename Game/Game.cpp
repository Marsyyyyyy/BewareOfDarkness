#include "Game.hpp"
#include "Constants.hpp"

Game::Game(sf::RenderWindow& window, const GameSettings& settings,
    std::function<void()> victoryCallback,
    std::function<void()> gameOverCallback) :
    window(window),
    settings(settings),
    scene(window.getSize(), settings, victoryCallback, gameOverCallback),
    camera(window.getSize()),
    onVictory(victoryCallback)
{
    // Game.cpp - ajuster les bounds pour bloquer la caméra au sol
    camera.setLevelBounds(sf::FloatRect({ 0.f, 0.f }, { 4000.f, 1080.f }));
}

void Game::update(float dt)
{
    scene.update(dt);

    // Utiliser le joueur de Scene
    camera.update(
        dt,
        scene.getPlayerPosition(),
        scene.getPlayerVelocity().x
    );
}

void Game::render(sf::RenderWindow& window)
{
    scene.render(window);        // background fixe géré dedans
    camera.apply(window);        // appliquer APRÈS le background
    scene.renderWorld(window);   // joueur, plateformes, ennemis
}

void Game::processEvents()
{
    while (auto event = window.pollEvent())
        if (event->is<sf::Event::Closed>())
            window.close();
}