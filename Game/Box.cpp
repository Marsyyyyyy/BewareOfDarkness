#include "Box.hpp"
#include "Constants.hpp"
#include <iostream>

Box::Box(sf::Vector2f pos)
{
    shape.setSize({ 80.f, 80.f });
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(180, 60, 0));
}

void Box::update(float dt, std::vector<Platform>& platforms,
    sf::FloatRect playerBounds, sf::Vector2f playerVel)
{
    velocity.y += Constants::GRAVITY * dt;

    // Reset velocity X chaque frame
    velocity.x = 0.f;

    auto intersection = shape.getGlobalBounds().findIntersection(playerBounds);
    if (intersection)
    {
        float playerBottom = playerBounds.position.y + playerBounds.size.y;
        float boxTop = shape.getPosition().y;
        bool playerOnTop = playerBottom < boxTop + 8.f;

        std::cout << "INTERSECTION! playerOnTop=" << playerOnTop
            << " playerVel.x=" << playerVel.x
            << " playerBottom=" << playerBottom
            << " boxTop=" << boxTop << "\n";

        if (!playerOnTop && std::abs(playerVel.x) > 10.f)
        {
            velocity.x = playerVel.x;
            std::cout << "PUSH! velocity.x=" << velocity.x << "\n";
        }
    }

    // Déplacement X
    shape.move(sf::Vector2f(velocity.x * dt, 0.f));
    for (auto& p : platforms)
    {
        auto inter = shape.getGlobalBounds().findIntersection(p.getBounds());
        if (inter)
        {
            if (velocity.x > 0) shape.move(sf::Vector2f(-inter->size.x, 0.f));
            if (velocity.x < 0) shape.move(sf::Vector2f(inter->size.x, 0.f));
            velocity.x = 0;
        }
    }

    // Déplacement Y
    shape.move(sf::Vector2f(0.f, velocity.y * dt));
    isGrounded = false;
    for (auto& p : platforms)
    {
        auto inter = shape.getGlobalBounds().findIntersection(p.getBounds());
        if (inter)
        {
            if (velocity.y > 0) { shape.move(sf::Vector2f(0.f, -inter->size.y)); velocity.y = 0; isGrounded = true; }
            if (velocity.y < 0) { shape.move(sf::Vector2f(0.f, inter->size.y)); velocity.y = 0; }
        }
    }
}

void Box::render(sf::RenderWindow& window)
{
    window.draw(shape);

    sf::RectangleShape line1({ 70.f, 6.f });
    line1.setFillColor(sf::Color::Red);
    line1.setOrigin({ 35.f, 3.f });
    line1.setPosition(shape.getPosition() + sf::Vector2f(40.f, 40.f));
    line1.setRotation(sf::degrees(45));

    sf::RectangleShape line2({ 70.f, 6.f });
    line2.setFillColor(sf::Color::Red);
    line2.setOrigin({ 35.f, 3.f });
    line2.setPosition(shape.getPosition() + sf::Vector2f(40.f, 40.f));
    line2.setRotation(sf::degrees(-45));

    window.draw(line1);
    window.draw(line2);
}