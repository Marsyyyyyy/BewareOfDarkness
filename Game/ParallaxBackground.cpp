#include "ParallaxBackground.hpp"
#include "ResourceManager.hpp"

ParallaxBackground::ParallaxBackground(sf::Vector2u winSize) : windowSize(winSize)
{
    auto& rm = ResourceManager::getInstance();
    float w = (float)winSize.x;
    float h = (float)winSize.y;

    layers.reserve(3);
    layers.emplace_back(rm.loadTexture("bg1", "assets/background_1.png"), 0.05f, w, h);
    layers.emplace_back(rm.loadTexture("bg2", "assets/background_2.png"), 0.15f, w, h);
    layers.emplace_back(rm.loadTexture("bg3", "assets/background_3.png"), 0.30f, w, h);
}

void ParallaxBackground::update(float playerVelX, float dt)
{
    for (auto& layer : layers)
    {
        layer.offsetX -= playerVelX * layer.speed * dt;

        float w = layer.texture.getSize().x * layer.sprite1.getScale().x;

        if (layer.offsetX <= -w) layer.offsetX += w;
        if (layer.offsetX > 0) layer.offsetX -= w;

        layer.sprite1.setPosition({ layer.offsetX,     0.f });
        layer.sprite2.setPosition({ layer.offsetX + w, 0.f });
    }
}

void ParallaxBackground::render(sf::RenderWindow& window)
{
    sf::View gameView = window.getView();
    window.setView(window.getDefaultView());

    for (auto& layer : layers)
    {
        window.draw(layer.sprite1);
        window.draw(layer.sprite2);
    }

    window.setView(gameView);
}