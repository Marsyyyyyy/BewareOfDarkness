//#include "LightSystem.hpp"
//
//// Note: In SFML 3+, resize() returns a boolean
//LightSystem::LightSystem(sf::Vector2u windowSize)
//    : windowSize(windowSize)
//{
//    (void)lightMap.resize({ windowSize.x, windowSize.y });
//}
//
//void LightSystem::clearLights()
//{
//    lights.clear();
//    flashlights.clear();
//}
//
//void LightSystem::addLight(sf::Vector2f pos, float radius)
//{
//    lights.push_back({ pos,radius });
//}
//
//void LightSystem::addFlashlight(sf::Vector2f pos, float angle, float radius, float distance)
//{
//    flashlights.push_back({ pos, angle, radius, distance });
//}
//
//void LightSystem::render(sf::RenderWindow& window, const GameSettings& settings)
//{
//    // Map brightness (0-100) to a light intensity multiplier
//    // 0 -> 0.3x (very low, but visible)
//    // 70 -> 1.0x (normal)
//    // 100 -> 1.3x (bright)
//    float intensity = 0.3f + (settings.brightness / 100.0f) * 1.0f;
//
//    // 1. Clear the light map with a very dark ambient color (unlit areas remain pitch black)
//    lightMap.clear(sf::Color(5, 5, 5, 255));
//
//    // We must apply the camera view to the light map so the lights align with the world!
//    lightMap.setView(window.getView());
//
//    // 2. Draw all light auras (BlendNone or BlendAdd onto the dark map)
//    for (auto& l : lights)
//    {
//        // To make it incredibly smooth, we draw many low-opacity circles shrinking towards the center
//        int steps = 80;
//
//        // Accumulate fractional alpha to avoid truncation banding with varying intensities
//        float alphaPerStep = (160.0f * intensity) / steps;
//        float accumulator = 0.0f;
//
//        for (int i = 0; i < steps; ++i) {
//            float r = l.radius * (1.0f - (float)i / steps);
//            sf::CircleShape glow(r);
//            glow.setOrigin({ r, r });
//            glow.setPosition(l.position);
//
//            // Extract the integer portion of our accumulated alpha
//            accumulator += alphaPerStep;
//            int currentA = static_cast<int>(accumulator);
//            accumulator -= currentA;
//
//            if (currentA > 0) {
//                // Extremely low opacity per step, but they stack in the center to create a bright core
//                glow.setFillColor(sf::Color(255, 255, 255, static_cast<uint8_t>(std::min(255, currentA))));
//                lightMap.draw(glow, sf::BlendAdd);
//            }
//        }
//    }
//
//    // Draw all flashlights
//    for (auto& f : flashlights)
//    {
//        // Draw a true gradient cone using a VertexArray (TriangleFan)
//        // This gives a perfectly smooth fade from the flashlight out to the darkness
//        int beamSegments = 30; // Number of triangles to make up the curved end of the cone
//        sf::VertexArray beam(sf::PrimitiveType::TriangleFan, beamSegments + 2);
//
//        // Center point (at the player's flashlight) - Bright white, intensity scaled
//        uint8_t centerAlpha = static_cast<uint8_t>(std::min(255.0f, 120.0f * intensity));
//        beam[0].position = f.position;
//        beam[0].color = sf::Color(255, 255, 255, centerAlpha);
//
//        // The angle sweep of the flashlight cone (determines width based on radius vs distance)
//        float angleSweep = atan2(f.radius, f.distance);
//
//        // Convert the flashlight's aim angle from degrees to radians
//        float aimAngleRad = f.angle * 3.14159265f / 180.f;
//
//        for (int i = 0; i <= beamSegments; ++i)
//        {
//            // Interpolate from -angleSweep to +angleSweep around the base aim angle
//            float currentAngle = aimAngleRad - angleSweep + (2.0f * angleSweep * i / beamSegments);
//
//            // Calculate position of the outer edge vertex using trig
//            float xOffset = cos(currentAngle) * f.distance;
//            float yOffset = sin(currentAngle) * f.distance;
//
//            beam[i + 1].position = f.position + sf::Vector2f(xOffset, yOffset);
//
//            // Outer edge fades completely to transparent
//            beam[i + 1].color = sf::Color(255, 255, 255, 0);
//        }
//
//        lightMap.draw(beam, sf::BlendAdd);
//    }
//
//    // 3. Finish drawing to the texture
//    lightMap.display();
//
//    // 4. Draw the light map over the window using Multiply blending
//    sf::Sprite lightSprite(lightMap.getTexture());
//
//    // The lightSprite's texture maps exactly to what the camera sees.
//    // So its top-left corner should be exactly at the top-left of the camera's current view.
//    sf::Vector2f viewCenter = window.getView().getCenter();
//    sf::Vector2f viewSize = window.getView().getSize();
//    lightSprite.setPosition(viewCenter - viewSize / 2.f);
//
//    // In SFML, render textures might be flipped depending on OpenGL context, 
//    // but usually sprites handle this. However, since the view was applied to the RenderTexture,
//    // we want to ensure its scale matches the view size if the view is zoomed
//    lightSprite.setScale({ viewSize.x / lightMap.getSize().x, viewSize.y / lightMap.getSize().y });
//
//    window.draw(lightSprite, sf::BlendMultiply);
//}