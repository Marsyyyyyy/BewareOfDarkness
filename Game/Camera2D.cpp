#include "Camera2D.hpp"

Camera2D::Camera2D(sf::Vector2u windowSize)
{
    view.setSize(sf::Vector2f(windowSize));
    view.setCenter(sf::Vector2f(windowSize) / 2.f);

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

    sf::FloatRect currentDeadZone(
        { camCenter.x - deadZone.size.x / 2.f,
         camCenter.y - deadZone.size.y / 2.f },
        deadZone.size
    );

    sf::Vector2f target = camCenter;

    if (playerPos.x < currentDeadZone.position.x)
        target.x = playerPos.x + deadZone.size.x / 2.f;

    if (playerPos.x > currentDeadZone.position.x + currentDeadZone.size.x)
        target.x = playerPos.x - deadZone.size.x / 2.f;

    if (playerPos.y < currentDeadZone.position.y)
        target.y = playerPos.y + deadZone.size.y / 2.f;

    if (playerPos.y > currentDeadZone.position.y + currentDeadZone.size.y)
        target.y = playerPos.y - deadZone.size.y / 2.f;

    if (playerVelocityX > 0)
        lookAhead.x = 100.f;
    else if (playerVelocityX < 0)
        lookAhead.x = -100.f;

    target += lookAhead;

    camCenter += (target - camCenter) * smoothSpeed * dt;

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