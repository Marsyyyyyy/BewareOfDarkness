#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class ParallaxBackground
{
private:
    struct Layer {
        sf::Texture texture;
        sf::Sprite  sprite1;
        sf::Sprite  sprite2;
        float       speed;
        float       offsetX = 0.f;

        // Constructeur pour initialiser les sprites avec la texture
        Layer(sf::Texture tex, float spd, float winW, float winH)
            : texture(std::move(tex))
            , sprite1(texture)
            , sprite2(texture)
            , speed(spd)
        {
            float scaleY = winH / texture.getSize().y;
            float scaleX = winW / texture.getSize().x;
            float scale = std::max(scaleX, scaleY);

            sprite1.setScale({ scale, scale });
            sprite2.setScale({ scale, scale });

            float w = texture.getSize().x * scale;
            sprite2.setPosition({ w, 0.f });
        }
    };

    std::vector<Layer> layers;
    sf::Vector2u windowSize;

public:
    ParallaxBackground(sf::Vector2u winSize);
    void update(float playerVelX, float dt);
    void render(sf::RenderWindow& window);
};