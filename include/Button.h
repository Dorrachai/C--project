#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class Button {
public:
    Button(const std::string& text, float x, float y, sf::Font& font, int fontSize = 20);
    void draw(sf::RenderWindow& window);
    bool isHovered(sf::Vector2i mousePosition) const;
    bool isClicked(const sf::Event& event, const sf::RenderWindow& window) const;
    void setFont(sf::Font& font);

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
};

#endif
