#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

// Direction du son par rapport au joueur
enum class SoundDirection
{
    Above,      // au dessus
    Below,      // en dessous
    Left,       // a gauche
    Right,      // a droite
    Behind,     // derriere (bas de l'ecran)
    Front       // devant   (haut de l'ecran)
};

// Type de son (pour couleur + icone)
enum class SoundType
{
    MonsterReal,   // vrai monstre qui spawn
    MonsterBait,   // faux son / bait
    Ambient,       // son d'ambiance
    Danger         // danger imminent
};

struct SoundAlert
{
    SoundDirection direction;
    SoundType      type;
    float          lifetime = 0.f;     // temps restant
    float          maxLife = 2.0f;    // duree totale
    float          intensity = 1.0f;   // force du son (taille indicateur)
};

class SoundIndicator
{
public:
    SoundIndicator(sf::Vector2u windowSize, const sf::Font& font)
        : m_windowSize(windowSize)
        , m_font(font)
    {
    }

    // Appeler quand un son se produit dans le jeu
    void trigger(SoundDirection dir, SoundType type,
        float intensity = 1.f, float duration = 2.f)
    {
        SoundAlert alert;
        alert.direction = dir;
        alert.type = type;
        alert.lifetime = duration;
        alert.maxLife = duration;
        alert.intensity = intensity;
        m_alerts.push_back(alert);
    }

    void update(float dt)
    {
        for (auto& a : m_alerts)
            a.lifetime -= dt;

        // Supprimer les alertes expirees
        m_alerts.erase(
            std::remove_if(m_alerts.begin(), m_alerts.end(),
                [](const SoundAlert& a) { return a.lifetime <= 0.f; }),
            m_alerts.end());
    }

    void draw(sf::RenderTarget& target) const
    {
        for (const auto& alert : m_alerts)
            drawAlert(target, alert);
    }

    bool hasAlerts() const { return !m_alerts.empty(); }

private:
    sf::Color getColor(SoundType type) const
    {
        switch (type)
        {
        case SoundType::MonsterReal: return sf::Color(255, 50, 50, 255);  // rouge
        case SoundType::MonsterBait: return sf::Color(255, 200, 0, 255);  // jaune/orange
        case SoundType::Ambient:     return sf::Color(100, 100, 255, 255);  // bleu doux
        case SoundType::Danger:      return sf::Color(255, 0, 0, 255);  // rouge vif
        default:                     return sf::Color(255, 255, 255, 255);
        }
    }

    std::string getLabel(SoundType type) const
    {
        switch (type)
        {
        case SoundType::MonsterReal: return "!  MONSTER  !";
        case SoundType::MonsterBait: return "?  sound  ?";
        case SoundType::Ambient:     return "~ ambient ~";
        case SoundType::Danger:      return "!! DANGER !!";
        default:                     return "???";
        }
    }

    sf::Vector2f getPosition(SoundDirection dir) const
    {
        float cx = m_windowSize.x / 2.f;
        float cy = m_windowSize.y / 2.f;
        float margin = 80.f;

        switch (dir)
        {
        case SoundDirection::Above:  return { cx, margin };
        case SoundDirection::Below:  return { cx, m_windowSize.y - margin };
        case SoundDirection::Left:   return { margin, cy };
        case SoundDirection::Right:  return { m_windowSize.x - margin, cy };
        case SoundDirection::Behind: return { cx, m_windowSize.y - margin };
        case SoundDirection::Front:  return { cx, margin };
        default:                     return { cx, cy };
        }
    }

    float getRotation(SoundDirection dir) const
    {
        switch (dir)
        {
        case SoundDirection::Above:
        case SoundDirection::Front:  return 0.f;      // pointe vers le haut
        case SoundDirection::Below:
        case SoundDirection::Behind: return 180.f;     // pointe vers le bas
        case SoundDirection::Left:   return -90.f;     // pointe a gauche
        case SoundDirection::Right:  return 90.f;      // pointe a droite
        default:                     return 0.f;
        }
    }

    void drawAlert(sf::RenderTarget& target, const SoundAlert& alert) const
    {
        float alpha = std::clamp(alert.lifetime / alert.maxLife, 0.f, 1.f);
        // Pulse rapide pour attirer l'attention
        float pulse = std::sin(alert.lifetime * 8.f) * 0.3f + 0.7f;
        float finalAlpha = alpha * pulse;

        sf::Color col = getColor(alert.type);
        sf::Vector2f pos = getPosition(alert.direction);
        float rot = getRotation(alert.direction);

        // -- Fleche directionnelle --
        float triSize = 20.f * alert.intensity;
        sf::CircleShape arrow(triSize, 3);
        arrow.setOrigin({ triSize, triSize });
        arrow.setPosition(pos);
        arrow.setRotation(sf::degrees(rot));
        arrow.setFillColor({ col.r, col.g, col.b,
            static_cast<uint8_t>(220 * finalAlpha) });
        target.draw(arrow);

        // -- Glow derriere la fleche --
        sf::CircleShape glow(triSize * 2.f);
        glow.setOrigin({ triSize * 2.f, triSize * 2.f });
        glow.setPosition(pos);
        glow.setFillColor({ col.r, col.g, col.b,
            static_cast<uint8_t>(30 * finalAlpha) });
        target.draw(glow);

        // -- Texte descriptif --
        sf::Text label(m_font, getLabel(alert.type), 16u);
        label.setLetterSpacing(2.f);
        label.setFillColor({ col.r, col.g, col.b,
            static_cast<uint8_t>(200 * finalAlpha) });
        auto lb = label.getLocalBounds();
        label.setOrigin({ lb.position.x + lb.size.x / 2.f,
                          lb.position.y + lb.size.y / 2.f });

        // Decaler le texte par rapport a la fleche
        sf::Vector2f textOffset{ 0.f, 0.f };
        switch (alert.direction)
        {
        case SoundDirection::Above:
        case SoundDirection::Front:  textOffset.y = 40.f;  break;
        case SoundDirection::Below:
        case SoundDirection::Behind: textOffset.y = -40.f; break;
        case SoundDirection::Left:   textOffset.x = 50.f;  break;
        case SoundDirection::Right:  textOffset.x = -50.f; break;
        }
        label.setPosition(pos + textOffset);
        target.draw(label);

        // -- Barre de danger pour MonsterReal et Danger --
        if (alert.type == SoundType::MonsterReal ||
            alert.type == SoundType::Danger)
        {
            // Flash rouge sur le bord de l'ecran
            drawEdgeFlash(target, alert.direction, col, finalAlpha);
        }
    }

    void drawEdgeFlash(sf::RenderTarget& target, SoundDirection dir,
        sf::Color col, float alpha) const
    {
        sf::RectangleShape flash;
        float thickness = 6.f;
        float w = static_cast<float>(m_windowSize.x);
        float h = static_cast<float>(m_windowSize.y);

        switch (dir)
        {
        case SoundDirection::Above:
        case SoundDirection::Front:
            flash.setSize({ w, thickness });
            flash.setPosition({ 0.f, 0.f });
            break;
        case SoundDirection::Below:
        case SoundDirection::Behind:
            flash.setSize({ w, thickness });
            flash.setPosition({ 0.f, h - thickness });
            break;
        case SoundDirection::Left:
            flash.setSize({ thickness, h });
            flash.setPosition({ 0.f, 0.f });
            break;
        case SoundDirection::Right:
            flash.setSize({ thickness, h });
            flash.setPosition({ w - thickness, 0.f });
            break;
        }

        flash.setFillColor({ col.r, col.g, col.b,
            static_cast<uint8_t>(120 * alpha) });
        target.draw(flash);
    }

    sf::Vector2u              m_windowSize;
    const sf::Font& m_font;
    std::vector<SoundAlert>   m_alerts;
};