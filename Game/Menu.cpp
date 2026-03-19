#include "Menu.hpp"

// ================================================================
//  NeonGlowText
// ================================================================

NeonGlowText::NeonGlowText(const sf::Font& font, const std::string& str,
    unsigned int size, sf::Color glowColor)
    : m_font(font)
    , m_glowColor(glowColor)
    , m_coreText(font, str, size)
{
    m_coreText.setFillColor(sf::Color(255, 255, 255, 255));
    m_coreText.setLetterSpacing(6.f);

    for (int i = 0; i < m_layers; ++i)
    {
        sf::Text layer(font, str, size);
        layer.setLetterSpacing(6.f);

        float   t = static_cast<float>(i) / m_layers;
        uint8_t a = static_cast<uint8_t>((1.f - t) * 140.f);
        layer.setFillColor({ glowColor.r, glowColor.g, glowColor.b, a });

        m_glowLayers.push_back(std::move(layer));
    }
}

void NeonGlowText::setPosition(sf::Vector2f pos)
{
    m_position = pos;

    auto cb = m_coreText.getLocalBounds();
    m_coreText.setOrigin({ cb.position.x + cb.size.x / 2.f,
                           cb.position.y + cb.size.y / 2.f });
    m_coreText.setPosition(pos);

    for (auto& layer : m_glowLayers)
    {
        auto lb = layer.getLocalBounds();
        layer.setOrigin({ lb.position.x + lb.size.x / 2.f,
                          lb.position.y + lb.size.y / 2.f });
        layer.setPosition(pos);
    }
}

void NeonGlowText::update(float time)
{
    m_time = time;
    float pulse = std::sin(time * 2.f) * 0.15f + 0.85f;

    for (int i = 0; i < m_layers; ++i)
    {
        float   t = static_cast<float>(i) / m_layers;
        float   scale = 1.f + t * 0.10f * pulse;
        uint8_t a = static_cast<uint8_t>((1.f - t) * 100.f * pulse);

        m_glowLayers[i].setScale({ scale, scale });
        m_glowLayers[i].setFillColor({ m_glowColor.r, m_glowColor.g,
                                       m_glowColor.b, a });
    }

    float   corePulse = std::sin(time * 3.f) * 0.1f + 0.9f;
    uint8_t ca = static_cast<uint8_t>(230 + 25 * corePulse);
    m_coreText.setFillColor({ 255, 255, 255, ca });
}

void NeonGlowText::draw(sf::RenderTarget& target) const
{
    for (int i = m_layers - 1; i >= 0; --i)
        target.draw(m_glowLayers[i]);
    target.draw(m_coreText);
}

// ================================================================
//  NeonButton
// ================================================================

NeonButton::NeonButton(const sf::Font& font, const std::string& label,
    sf::Vector2f position, sf::Color neonColor)
    : m_label(font, label, 32u)
    , m_neonColor(neonColor)
    , m_position(position)
{
    m_label.setFillColor(NeonColors::DimWhite);
    m_label.setLetterSpacing(3.f);

    auto bounds = m_label.getLocalBounds();
    m_label.setOrigin({ bounds.position.x + bounds.size.x / 2.f,
                        bounds.position.y + bounds.size.y / 2.f });
    m_label.setPosition(position);

    m_box.setSize({ 320.f, 60.f });
    m_box.setOrigin({ 160.f, 30.f });
    m_box.setPosition(position);
    m_box.setFillColor(sf::Color(15, 15, 30, 180));
    m_box.setOutlineThickness(2.f);
    m_box.setOutlineColor(sf::Color(neonColor.r, neonColor.g, neonColor.b, 100));

    m_glow.setSize({ 340.f, 72.f });
    m_glow.setOrigin({ 170.f, 36.f });
    m_glow.setPosition(position);
    m_glow.setFillColor(sf::Color(neonColor.r, neonColor.g, neonColor.b, 0));
}

void NeonButton::update(bool selected, float dt, float totalTime)
{
    float target = selected ? 1.f : 0.f;
    m_selectT += (target - m_selectT) * dt * 10.f;
    m_selectT = std::clamp(m_selectT, 0.f, 1.f);

    float pulse = std::sin(totalTime * 3.f) * 0.15f + 0.85f;

    auto lerp8 = [](uint8_t a, uint8_t b, float t) -> uint8_t {
        return static_cast<uint8_t>(a + (b - a) * t);
        };

    m_label.setFillColor({
        lerp8(180, m_neonColor.r, m_selectT),
        lerp8(180, m_neonColor.g, m_selectT),
        lerp8(200, m_neonColor.b, m_selectT), 255 });

    float s = 1.f + m_selectT * 0.08f;
    m_label.setScale({ s, s });

    auto oa = static_cast<uint8_t>(40 + m_selectT * 215 * pulse);
    m_box.setOutlineColor({ m_neonColor.r, m_neonColor.g, m_neonColor.b, oa });
    m_box.setOutlineThickness(1.5f + m_selectT * 2.f);

    auto ba = static_cast<uint8_t>(
        std::clamp((10.f + m_selectT * 35.f) * 4.f, 0.f, 255.f));
    m_box.setFillColor({
        static_cast<uint8_t>(m_neonColor.r / 8),
        static_cast<uint8_t>(m_neonColor.g / 8),
        static_cast<uint8_t>(m_neonColor.b / 8), ba });

    auto ga = static_cast<uint8_t>(
        std::clamp(m_selectT * 35.f * pulse, 0.f, 255.f));
    m_glow.setFillColor({ m_neonColor.r, m_neonColor.g, m_neonColor.b, ga });
}

void NeonButton::draw(sf::RenderTarget& rt) const
{
    rt.draw(m_glow);
    rt.draw(m_box);
    rt.draw(m_label);
}

// ================================================================
//  NeonMenu
// ================================================================

NeonMenu::NeonMenu(sf::Vector2u windowSize, const sf::Font& font)
    : m_font(font)
    , m_windowSize(windowSize)
    , m_title(font, "Défaillant", 66u, sf::Color(0, 255, 255))
    , m_subtitle(font, "INTO  THE  UNKNOWN", 22u)
    , m_hint(font, "UP / DOWN   -   ENTER", 14u)
    , m_footer(font, "2026  -  NINI DEV", 13u)
{
    float cx = windowSize.x / 2.f;
    float startY = windowSize.y * 0.46f;
    float gap = 85.f;

    m_buttons.emplace_back(font, "PLAY",
        sf::Vector2f{ cx, startY }, NeonColors::Cyan);
    m_buttons.emplace_back(font, "OPTIONS",
        sf::Vector2f{ cx, startY + gap }, NeonColors::Pink);
    m_buttons.emplace_back(font, "EXIT",
        sf::Vector2f{ cx, startY + gap * 2.f }, NeonColors::Purple);

    // Titre glow
    m_title.setPosition({ cx, windowSize.y * 0.15f });

    // Sous-titre
    m_subtitle.setLetterSpacing(8.f);
    m_subtitle.setFillColor(NeonColors::Pink);
    auto sb = m_subtitle.getLocalBounds();
    m_subtitle.setOrigin({ sb.position.x + sb.size.x / 2.f,
                           sb.position.y + sb.size.y / 2.f });
    m_subtitle.setPosition({ cx, windowSize.y * 0.25f });

    // Ligne neon
    m_line.setSize({ 400.f, 2.f });
    m_line.setOrigin({ 200.f, 1.f });
    m_line.setPosition({ cx, windowSize.y * 0.32f });
    m_line.setFillColor(sf::Color(0, 255, 255, 80));

    // Hint clavier
    m_hint.setLetterSpacing(4.f);
    m_hint.setFillColor(sf::Color(80, 80, 100, 120));
    auto hb = m_hint.getLocalBounds();
    m_hint.setOrigin({ hb.position.x + hb.size.x / 2.f,
                       hb.position.y + hb.size.y / 2.f });
    m_hint.setPosition({ cx, windowSize.y * 0.88f });

    // Footer
    m_footer.setLetterSpacing(4.f);
    m_footer.setFillColor(sf::Color(100, 100, 120, 150));
    auto fb = m_footer.getLocalBounds();
    m_footer.setOrigin({ fb.position.x + fb.size.x / 2.f,
                         fb.position.y + fb.size.y / 2.f });
    m_footer.setPosition({ cx, windowSize.y * 0.93f });

    initParticles(60);
}

void NeonMenu::moveUp()
{
    m_selected = (m_selected - 1 + static_cast<int>(m_buttons.size()))
        % static_cast<int>(m_buttons.size());
}

void NeonMenu::moveDown()
{
    m_selected = (m_selected + 1) % static_cast<int>(m_buttons.size());
}

NeonMenu::Action NeonMenu::confirm() const
{
    switch (m_selected)
    {
    case 0:  return Action::Play;
    case 1:  return Action::Options;
    case 2:  return Action::Exit;
    default: return Action::None;
    }
}

void NeonMenu::update(float dt)
{
    m_time += dt;

    for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
        m_buttons[i].update(i == m_selected, dt, m_time);

    // Titre glow
    m_title.update(m_time);

    // Sous-titre pulse
    float sp = std::sin(m_time * 1.5f + 1.f) * 0.3f + 0.7f;
    m_subtitle.setFillColor({ 255, 0, 128,
        static_cast<uint8_t>(140 + 80 * sp) });

    // Ligne pulse
    float lp = std::sin(m_time * 2.5f) * 0.4f + 0.6f;
    m_line.setFillColor({ 0, 255, 255,
        static_cast<uint8_t>(50 + 80 * lp) });

    // Hint pulse
    float hp = std::sin(m_time * 1.2f) * 0.3f + 0.7f;
    m_hint.setFillColor({ 80, 80, 100,
        static_cast<uint8_t>(60 + 60 * hp) });

    updateParticles(dt);
}

void NeonMenu::draw(sf::RenderTarget& target) const
{
    sf::RectangleShape bg{ toFloat(m_windowSize) };
    bg.setFillColor(NeonColors::DarkBg);
    target.draw(bg);

    drawParticles(target);
    target.draw(m_line);
    m_title.draw(target);
    target.draw(m_subtitle);
    for (const auto& btn : m_buttons)
        btn.draw(target);
    target.draw(m_hint);
    target.draw(m_footer);
}

void NeonMenu::initParticles(int count)
{
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dX(0.f, static_cast<float>(m_windowSize.x));
    std::uniform_real_distribution<float> dY(0.f, static_cast<float>(m_windowSize.y));
    std::uniform_real_distribution<float> dR(1.f, 3.5f);
    std::uniform_real_distribution<float> dS(8.f, 35.f);
    std::uniform_real_distribution<float> dA(0.2f, 0.7f);
    std::uniform_int_distribution<int>    dC(0, 2);

    sf::Color pal[] = { NeonColors::Cyan, NeonColors::Pink, NeonColors::Purple };

    for (int i = 0; i < count; ++i)
    {
        NeonParticle p;
        p.pos = { dX(rng), dY(rng) };
        p.radius = dR(rng);
        p.speed = dS(rng);
        p.alpha = dA(rng);
        p.baseColor = pal[dC(rng)];
        p.vel = { (dS(rng) - 20.f) * 0.5f, -p.speed * 0.3f };
        m_particles.push_back(p);
    }
}

void NeonMenu::updateParticles(float dt)
{
    float w = static_cast<float>(m_windowSize.x);
    float h = static_cast<float>(m_windowSize.y);

    for (auto& p : m_particles)
    {
        p.pos += p.vel * dt;
        if (p.pos.y < -10.f)    p.pos.y = h + 10.f;
        if (p.pos.x < -10.f)    p.pos.x = w + 10.f;
        if (p.pos.x > w + 10.f) p.pos.x = -10.f;
        p.alpha = 0.3f + 0.4f * std::sin(m_time * p.speed * 0.1f);
    }
}

void NeonMenu::drawParticles(sf::RenderTarget& target) const
{
    for (const auto& p : m_particles)
    {
        sf::CircleShape dot(p.radius);
        dot.setOrigin({ p.radius, p.radius });
        dot.setPosition(p.pos);

        auto a = static_cast<uint8_t>(
            std::clamp(p.alpha * 255.f, 0.f, 255.f));
        dot.setFillColor({ p.baseColor.r, p.baseColor.g,
                           p.baseColor.b, a });
        target.draw(dot);
    }
}