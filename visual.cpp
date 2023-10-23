#include "visual.hpp"
#include <SFML/Graphics.hpp>
#include <stdexcept>

namespace VirusGame {

Visual::Visual(sf::RenderWindow &window) : m_window{window}{
  if (!m_font.loadFromFile("include/INFECTION.ttf")) {
    if (!m_font.loadFromFile(
            "include/arialbi.ttf")) {
      throw std::runtime_error{"Cannot load font"};
    }
  } 
}

void Visual::draw(World const &world) {
  int const N = world.side();

  sf::Color Healedcolor(1,180,1); //red green blue
  sf::Color HealthyColor(1,100,255);
  sf::Color InfectedColor(250,1,1);         

  sf::Vector2f const cell_size{m_window.getSize() / static_cast<unsigned>(N)};//vettore 2 dim (x,y)
  //rettangolo celle: (0,0) in alto a sinistra

  sf::RectangleShape rect(cell_size);//rettangolo per cosmetica sulle celle
  rect.setFillColor(sf::Color::Black);
  rect.setOutlineThickness(1.0);
  rect.setOutlineColor(sf::Color::Black);

  for (int r = 0; r != N; ++r) {
    for (int c = 0; c != N; ++c) {
      rect.setPosition(c * cell_size.x, r * cell_size.y); //set posizione sulla finestra delle celle

      switch (world.Get_cell(r, c)) {
        case Cell::Healthy:
          rect.setFillColor(HealthyColor);
          break;

        case Cell::Infected:
          rect.setFillColor(InfectedColor);
          break;

        case Cell::Healed:
          rect.setFillColor(Healedcolor);
          break;

        case Cell::Dead:
          rect.setFillColor(sf::Color::Black);
          break;

        default:
          rect.setFillColor(sf::Color::White);
          break;
      }
      m_window.draw(rect);//disegno rettangolo sulla finestra
    }
  }
}

void Visual::show_message(std::string const &message) {
  sf::Text text{message, m_font};

  text.move((m_window.getSize().x - text.getLocalBounds().width) / 2, m_window.getSize().y / 2);

  sf::Color Textcolor = sf::Color::White;
  text.setFillColor(Textcolor);
  text.setOutlineColor(sf::Color::Black);
  text.setOutlineThickness(2.0);

  m_window.draw(text);//scrive un messaggio al centro della finestra
}
}  // namespace virus