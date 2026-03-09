#pragma once
// ============================================================
//  Settings.hpp  -  Structure des parametres du jeu
// ============================================================

#include <SFML/Window/Keyboard.hpp>
#include <string>

struct GameSettings
{
    // -- Controles --
    sf::Keyboard::Key moveUp = sf::Keyboard::Key::Z;
    sf::Keyboard::Key moveDown = sf::Keyboard::Key::S;
    sf::Keyboard::Key moveLeft = sf::Keyboard::Key::Q;
    sf::Keyboard::Key moveRight = sf::Keyboard::Key::D;
    sf::Keyboard::Key flashlight = sf::Keyboard::Key::Space;

    // -- Audio --
    int volume = 80;       // 0 - 100

    // -- Display --
    int brightness = 70;   // 0 - 100

    // -- Accessibilite --
    bool hearingMode = false;  // mode malentendant
};

// Helper : convertit une touche en texte lisible
inline std::string keyToString(sf::Keyboard::Key key)
{
    using K = sf::Keyboard::Key;
    switch (key)
    {
    case K::A: return "A";   case K::B: return "B";
    case K::C: return "C";   case K::D: return "D";
    case K::E: return "E";   case K::F: return "F";
    case K::G: return "G";   case K::H: return "H";
    case K::I: return "I";   case K::J: return "J";
    case K::K: return "K";   case K::L: return "L";
    case K::M: return "M";   case K::N: return "N";
    case K::O: return "O";   case K::P: return "P";
    case K::Q: return "Q";   case K::R: return "R";
    case K::S: return "S";   case K::T: return "T";
    case K::U: return "U";   case K::V: return "V";
    case K::W: return "W";   case K::X: return "X";
    case K::Y: return "Y";   case K::Z: return "Z";
    case K::Num0: return "0"; case K::Num1: return "1";
    case K::Num2: return "2"; case K::Num3: return "3";
    case K::Num4: return "4"; case K::Num5: return "5";
    case K::Num6: return "6"; case K::Num7: return "7";
    case K::Num8: return "8"; case K::Num9: return "9";
    case K::Space:  return "SPACE";
    case K::Enter:  return "ENTER";
    case K::Escape: return "ESC";
    case K::LShift: return "L-SHIFT";
    case K::RShift: return "R-SHIFT";
    case K::LControl: return "L-CTRL";
    case K::RControl: return "R-CTRL";
    case K::Tab:    return "TAB";
    case K::Up:     return "UP";
    case K::Down:   return "DOWN";
    case K::Left:   return "LEFT";
    case K::Right:  return "RIGHT";
    default:        return "???";
    }
}