#include <SFML/Graphics.hpp>
#include <string>
#include "iostream"
#include "visual.hpp"
#include "world.hpp"

int main() {
  int grid_dimension = 100;  
  int cell_size = 8;         
  int num_s = 4000;           
  int num_i = 1000;            
  double beta = 0.5;       
  double gamma = 0.09;
  double alfa = 0.05;        
  int framerate = 5;   

  std::string message1 = "no";
  std::cout << "Do you want to change grid dimensions and number of people?" << '\n'
            << "(Max dimension: 100x100 with cell size 8) (yes/no) ";
  std::cin >> message1;

  if (message1 == "yes") {
    std::cout << "Digit int grid size (<100): ";
    std::cin >> grid_dimension; std::cout << '\n';
    std::cout << "Digit int cell size: ";
    std::cin >> cell_size; std::cout << '\n';
    std::cout << "Digit int number of Susceptible: ";
    std::cin >> num_s; std::cout << '\n';
    std::cout << "Digit int number of Infected: ";
    std::cin >> num_i; std::cout << '\n';
  }

  std::string message2 = "no";
  std::cout << "Do you want to change probability parameters? (yes/no) ";
  std::cin >> message2;

  if (message2 == "yes") {
    std::cout << "Digit double beta (0<beta<1): ";
    std::cin >> beta; std::cout << '\n';
    std::cout << "Digit double gamma (0<gamma<1): ";
    std::cin >> gamma; std::cout << '\n';
    std::cout << "Digit double alfa (0<alfa<1): ";
    std::cin >> alfa; std::cout << '\n';
  }       

  constexpr auto window_title{"Virus Game"};

  sf::RenderWindow window{sf::VideoMode(grid_dimension * cell_size, grid_dimension * cell_size),
      window_title, sf::Style::Close};

  window.setFramerateLimit(framerate);

  Virus::Display display{window};

  Virus::World world(grid_dimension);
  Virus::initial_random(world, num_s, num_i);

  if (window.isOpen()) {
    window.clear(sf::Color::White);
    display.draw(world);
    display.show_message("Press any key to start/stop");
    window.display();

    sf::Event event;
    while (window.waitEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        break;
      } else if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
  }

  bool is_running = true;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        is_running = !is_running;
      } else if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::White);

    if (is_running) {
      world = evolve(world, beta, gamma, alfa);
      display.draw(world);
    } else {
      display.draw(world);
      display.show_message("Pause");
    }

    if (!Virus::virus_condition(world)) {
      break;
    }

    window.display();
  }

  if (window.isOpen()) {
    int r_counter = world.R_Number();
    int d_counter = world.D_Number();
    int N_people = num_i + num_s;
    int survivors = N_people - d_counter;
    int infected = d_counter + r_counter - num_i;//new infected during virus
    
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("../include/Roboto-Regular.ttf")) {
      throw std::runtime_error("Can't load Roboto font");
    }

    text.setFont(font);
    text.setCharacterSize(20);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Black);
    text.setPosition(10, 10);
  
    sf::Vector2f size(385, 160);
    sf::RectangleShape rect(size);
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(3.0);
    rect.setPosition(6, 6);

    std::string counter_message =
        "Number of initial susceptibles: " + std::to_string(num_s) + '\n' +
        "Number of initial infected: " + std::to_string(num_i) + '\n' +
        "Number of recovered: " + std::to_string(r_counter) + '\n' +
        "Number of dead: " + std::to_string(d_counter) + '\n' +
        "Number of survivors: " + std::to_string(survivors) + '\n' +
        "Number of infected during virus: " + std::to_string(infected);

    text.setString(counter_message);

    window.clear(sf::Color::White);
    display.draw(world);
    display.show_message("Epidemy is over");

    window.draw(rect);
    window.draw(text);
       
    window.display();

    sf::Event event;
    while (window.waitEvent(event)) {
      if (event.type == sf::Event::KeyPressed ||
          event.type == sf::Event::Closed) {
        window.close();
      }
    }
  }
  
}