#include "LightSystem.hpp"
#include <iostream>

LightSystem::LightSystem(sf::Vector2u windowSize)
    : windowSize(windowSize)
{
    (void)lightMap.resize({ windowSize.x, windowSize.y });
}

void LightSystem::clearLights()
{
    lights.clear();
    flashlights.clear();
}

void LightSystem::addLight(sf::Vector2f pos, float radius)
{
    lights.push_back({ pos, radius });
}

void LightSystem::addFlashlight(sf::Vector2f pos, float angle, float radius, float distance, bool violet)
{
    flashlights.push_back({ pos, angle, radius, distance, violet });
}

void LightSystem::render(sf::RenderWindow& window, const GameSettings& settings)
{   
    float intensity = 0.3f + (settings.brightness / 100.0f) * 1.0f;

    // 1. Ambiance très sombre — quasi noire pour plus de contraste
    lightMap.clear(sf::Color(2, 2, 5, 255));
    lightMap.setView(window.getView());

    // 2. Petite lumière chaude autour du joueur (halo de proximité)
    for (auto& l : lights)
    {
        int steps = 40; // moins de steps = plus rapide
        float alphaPerStep = (140.0f * intensity) / steps;
        float accumulator = 0.0f;

        for (int i = 0; i < steps; ++i)
        {
            float r = l.radius * (1.0f - (float)i / steps);
            sf::CircleShape glow(r);
            glow.setOrigin({ r, r });
            glow.setPosition(l.position);

            accumulator += alphaPerStep;
            int currentA = static_cast<int>(accumulator);
            accumulator -= currentA;

            if (currentA > 0)
            {
                // Couleur chaude orange/ambre pour le halo
                glow.setFillColor(sf::Color(255, 140, 30, static_cast<uint8_t>(std::min(255, currentA))));
                lightMap.draw(glow, sf::BlendAdd);
            }
        }
    }

    // 3. Cone de flashlight avec bords très progressifs
    for (auto& f : flashlights)
    {
        float aimAngleRad = f.angle * 3.14159265f / 180.f;
        float angleSweep = atan2(f.radius, f.distance);

        // Couleur selon mode
        std::cout << "violet=" << f.violet << "\n";
        sf::Color coneColor = f.violet
            ? sf::Color(180, 0, 255, 255)  // violet
            : sf::Color(255, 255, 255, 255);

        struct ConePass { float sweepMult; float alphaMult; sf::Color color; };
        std::vector<ConePass> passes = f.violet ? std::vector<ConePass>{
            // Passes violettes
            { 2.5f, 0.08f, sf::Color(180, 0, 255, 255) },
            { 1.8f, 0.15f, sf::Color(190, 0,   255, 255) },
            { 1.2f, 0.25f, sf::Color(200, 20,  255, 255) },
            { 1.0f, 0.45f, sf::Color(210, 40,  255, 255) },
            { 0.5f, 0.6f,  sf::Color(220, 80,  255, 255) },
            { 0.2f, 0.4f,  sf::Color(230, 120, 255, 255) },
        } : std::vector<ConePass>{
                // Passes normales (existantes)
                { 2.5f, 0.08f, sf::Color(255, 200, 80,  255) },
                { 1.8f, 0.15f, sf::Color(255, 210, 100, 255) },
                { 1.2f, 0.25f, sf::Color(255, 220, 130, 255) },
                { 1.0f, 0.45f, sf::Color(255, 230, 160, 255) },
                { 0.5f, 0.6f,  sf::Color(255, 245, 200, 255) },
                { 0.2f, 0.4f,  sf::Color(255, 255, 230, 255) },
        };

        int beamSegments = 120;

        for (auto& pass : passes)
        {
            sf::VertexArray beam(sf::PrimitiveType::TriangleFan, beamSegments + 2);

            uint8_t centerAlpha = static_cast<uint8_t>(
                std::min(255.0f, pass.alphaMult * 255.f * intensity));

            beam[0].position = f.position;
            beam[0].color = sf::Color(pass.color.r, pass.color.g, pass.color.b, centerAlpha);

            float sweep = angleSweep * pass.sweepMult;

            for (int i = 0; i <= beamSegments; ++i)
            {
                float currentAngle = aimAngleRad - sweep
                    + (2.0f * sweep * i / beamSegments);

                float xOffset = cos(currentAngle) * f.distance;
                float yOffset = sin(currentAngle) * f.distance;

                beam[i + 1].position = f.position + sf::Vector2f(xOffset, yOffset);
                beam[i + 1].color = sf::Color(pass.color.r, pass.color.g, pass.color.b, 0);
            }

            lightMap.draw(beam, sf::BlendAdd);
        }

        // Petite lumière ponctuelle à l'origine du cone (lampe elle-même)
        int glowSteps = 20;
        for (int i = 0; i < glowSteps; ++i)
        {
            float r = 30.f * (1.0f - (float)i / glowSteps);
            sf::CircleShape glow(r);
            glow.setOrigin({ r, r });
            glow.setPosition(f.position);
            uint8_t a = static_cast<uint8_t>((80.f * intensity) / glowSteps);
            glow.setFillColor(sf::Color(255, 220, 120, a));
            lightMap.draw(glow, sf::BlendAdd);
        }
    }

    // Voile violet si mode actif
    if (flashlights.size() > 0 && flashlights[0].violet)
    {
        sf::RectangleShape veil(sf::Vector2f((float)windowSize.x, (float)windowSize.y));
        veil.setFillColor(sf::Color(60, 0, 100, 255));

        // Dessiner en coordonnées monde (vue caméra)
        sf::Vector2f viewCenter = window.getView().getCenter();
        sf::Vector2f viewSize = window.getView().getSize();
        veil.setSize(viewSize);
        veil.setPosition(viewCenter - viewSize / 2.f);
        lightMap.draw(veil, sf::BlendAdd);
    }

    // 4. Finaliser et dessiner
    lightMap.display();

    sf::Sprite lightSprite(lightMap.getTexture());
    sf::Vector2f viewCenter = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();
    lightSprite.setPosition(viewCenter - viewSize / 2.f);
    lightSprite.setScale({
        viewSize.x / lightMap.getSize().x,
        viewSize.y / lightMap.getSize().y
        });

    window.draw(lightSprite, sf::BlendMultiply);
}