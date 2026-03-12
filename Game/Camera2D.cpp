#include "Camera2D.hpp"

Camera2D::Camera2D(sf::Vector2u windowSize)
{
    // A bit less zoomed than before, so we see more of the level.
    view.setSize(sf::Vector2f(windowSize) / 1.3f);
    view.setCenter(sf::Vector2f(windowSize) / 2.6f);

    smoothSpeed = 6.f;

    deadZone = sf::FloatRect({ 0.f,0.f }, { 120.f,80.f });

    lookAhead = { 0.f,0.f };
}

void Camera2D::setLevelBounds(const sf::FloatRect& bounds)
{
    levelBounds = bounds;
}

void Camera2D::update(float dt, const sf::Vector2f& playerPos, float playerVelocityX)
{
    sf::Vector2f camCenter = view.getCenter();

    // Directly track the player's position without any deadzone or delay
    sf::Vector2f target = playerPos;

    // Shift camera UP so the player appears near the bottom of the screen.
    // Reduced from -250.f to slightly lower the camera.
    target.y -= 180.f;

    // Instantly snap to target instead of smoothing
    camCenter = target;

    float halfW = view.getSize().x / 2.f;
    float halfH = view.getSize().y / 2.f;

    if (camCenter.x - halfW < levelBounds.position.x)
        camCenter.x = levelBounds.position.x + halfW;

    if (camCenter.x + halfW > levelBounds.position.x + levelBounds.size.x)
        camCenter.x = levelBounds.position.x + levelBounds.size.x - halfW;

    if (camCenter.y - halfH < levelBounds.position.y)
        camCenter.y = levelBounds.position.y + halfH;

    if (camCenter.y + halfH > levelBounds.position.y + levelBounds.size.y)
        camCenter.y = levelBounds.position.y + levelBounds.size.y - halfH;

    view.setCenter(camCenter);
}

void Camera2D::apply(sf::RenderWindow& window)
{
    window.setView(view);
}

sf::View& Camera2D::getView()
{
    return view;
}