#include <SFML/Graphics.hpp>
#include <string>
#include "iostream"
#include "visual.hpp"
#include "world.hpp"

int main() {
  int grid_dimension = 100;  
  int cell_size = 8;         
  int num_s = 3000;           
  int num_i = 800;            
  double beta = 0.5; //*20*beta*n infetti vicini        
  double gamma = 0.09;//100*gamma
  double alfa = 0.05;        
  int framerate = 5;         
  
  std::string message1 = "no";
  std::cout << "Do you want do change grid dimensions and people?";
  std::cin >> message1;
  if(message1 == "yes"){
    std::cout << "Digit an int grid size (<100): ";
    std::cin >> grid_dimension ; std::cout << '\n';
    std::cout << "Digit an int number of healthy: ";
    std::cin >> num_s ; std::cout << '\n';
    std::cout << "Digit an int number of infected: ";
    std::cin >> num_i ; std::cout << '\n';

  }

  std::string message2 = "no";
  std::cout << "Do you want do change virus parameters? (yes/no)";
  std::cin >> message2;
  if(message2 == "yes"){
    std::cout << "Digit a double beta parameter (0<beta<1): ";
    std::cin >> beta ; std::cout << '\n';
    std::cout << "Digit a double gamma parameter (0<gamma<1): ";
    std::cin >> gamma ; std::cout << '\n'; 
    std::cout << "Digit a double alfa parameter (0<alfa<1): ";
    std::cin >> alfa ; std::cout << '\n';
  }

  constexpr auto window_title{"Virus Game"};

  sf::RenderWindow window{sf::VideoMode(grid_dimension * cell_size, grid_dimension * cell_size),
      window_title, sf::Style::Close};//definisco un RenderWindow

  window.setFramerateLimit(framerate);//sleep di sfml per la finestra

  VirusGame::Visual display{window};//definisco un oggetto display attraverso la classe Visual
  //Oggetto display con 2 attributi privati: Finestra(renderwindow) e Font
  //qui utilizziamo il costruttore della classe

  VirusGame::World world(grid_dimension);
  VirusGame::initial_random(world, num_s, num_i);

  if (window.isOpen()) {
    window.clear(sf::Color::White);
    display.draw(world);//utilizzo il metodo creato per disegnare la griglia
    display.show_message("Press any key to start/stop");
    window.display();//mostra a schermo quanto c'è sulla finestra window (prima draw() poi display())

    sf::Event event;
    while (window.waitEvent(event)) { //waitEvent ritorna vero finchè non arriva un evento
      if (event.type == sf::Event::KeyPressed) {
        break;//qunado premo un tasto esco dal ciclo e la pandemia parte
      } else if (event.type == sf::Event::Closed) {
        window.close();
      }
    }//rimango qui dentro finchè non viene fatto partire il programma, poi esce dall'if
  }

  bool is_running = true;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) { //pollEvent ritorna falso finchè non arriva un evento
      if (event.type == sf::Event::KeyPressed) {
        is_running = !is_running;//se premo un tasto la pandemia si blocca (sblocco/blocco con tasto)
      } else if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::White);//pulisco la finestra ogni frame

    if (is_running) {
      world = evolve(world, beta, gamma, alfa);//se is_runnig è vero la pandemia continua
      display.draw(world);
    } else {
      display.draw(world);
      display.show_message("Pause");
    }
    //display.draw(world);

    if (!VirusGame::virus_condition(world)) {
      break;
    }//se gli infetti divengono zero la pandemia si ferma ed esco dal ciclo

    window.display(); //mostra a schermo il mondo nuovo disegnato prima sulla finestra
  }

  if (window.isOpen()) { //in questo if si entra dopo che i_counter=0
    int h_counter = world.H_Number();
    int d_counter = world.D_Number();
    int N_people = num_i + num_s;
    int survivors = N_people - d_counter;//sopravvissuti
    int infected = d_counter + h_counter - num_i;//numero infettati durante la pandemia
    
    sf::Text text;//testo per rettangolo di riepilogo
    sf::Font font;//font per rettangolo di riepilogo
    if (!font.loadFromFile("include/Roboto-Regular.ttf")) {
      throw std::runtime_error("can't load Roboto font");
    }
    text.setFont(font);//font setting
  
    text.setCharacterSize(20);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Black);
    text.setPosition(10, 10);
  
    sf::Vector2f size(385, 160);//vettore bidimensionale per il rettangolo di riepilogo
    sf::RectangleShape rect(size);//rettangolo analisi dati
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(3.0);
    rect.setPosition(6, 6);

    std::string counter_message =
        "Number of initial healthy: " + std::to_string(num_s) + '\n' +
        "Number of initial infected: " + std::to_string(num_i) + '\n' +
        "Number of healed people: " + std::to_string(h_counter) + '\n' +
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
    while (window.waitEvent(event)) {//la finestra viene chiusa se premo di nuovo un tasto o se la chiudo
      if (event.type == sf::Event::KeyPressed ||
          event.type == sf::Event::Closed) {
        window.close();
      }
    }
  }

  
}