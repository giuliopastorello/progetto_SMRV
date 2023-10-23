#ifndef VISUAL_HPP
#define VISUAL_HPP

#include <SFML/Graphics.hpp>
#include "world.hpp"

namespace VirusGame {

class Visual {
  sf::RenderWindow &m_window;
  sf::Font m_font;

 public:

  Visual(sf::RenderWindow &window);

  void draw(World const &world);

  void show_message(std::string const &message);
};
}// namespace VirusGame

#endif