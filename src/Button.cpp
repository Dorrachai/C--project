#include "../include/Button.h"

Button::Button(const std::string& text, float x, float y, sf::Font& font, int fontSize) {
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(fontSize);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition(x + 20.f, y + 10.f);

    buttonShape.setSize(sf::Vector2f(150.f, 50.f));
    buttonShape.setFillColor(sf::Color::White);
    buttonShape.setOutlineColor(sf::Color::Black);
    buttonShape.setOutlineThickness(2);
    buttonShape.setPosition(x, y);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(buttonShape);
    window.draw(buttonText);
}

void Button::setFont(sf::Font& font) {
    buttonText.setFont(font);
}

bool Button::isHovered(sf::Vector2i mousePosition) const {
    return buttonShape.getGlobalBounds().contains(
        static_cast<float>(mousePosition.x),
        static_cast<float>(mousePosition.y)
    );
}


bool Button::isClicked(const sf::Event& event, const sf::RenderWindow& window) const {
    if (event.type != sf::Event::MouseButtonPressed || event.mouseButton.button != sf::Mouse::Left)
        return false;

    auto mp = sf::Mouse::getPosition(window);
    return isHovered(mp);
}
