#include <SFML/Graphics.hpp>
#include <string>
#include "iostream"
#include "visual.hpp"
#include "world.hpp"

int main() {
  int grid_dimension = 50;  
  int cell_size = 15;         
  int num_s = 350;           
  int num_i = 160;            
  double beta = 0.4; //*20*beta*n infetti vicini        
  double gamma = 0.09;//100*gamma
  double alfa = 0.05;        
  int framerate = 5;         

  //manca ancora la parte dove si fanno inserire i dati manualmente dall'utente

  constexpr auto window_title{"Virus Game"};

  sf::RenderWindow window{
      sf::VideoMode(grid_dimension * cell_size, grid_dimension * cell_size),
      window_title, sf::Style::Close};//definisco un RenderWindow

  window.setFramerateLimit(framerate);//sleep di sfml per la finestra

  GameofLife::Visual display{window};//definisco un oggetto display attraverso la classe Visual
  //Oggetto display con 2 attributi privati: Finestra(renderwindow) e Font
  //qui utilizziamo il costruttore della classe

  GameofLife::World world(grid_dimension);
  GameofLife::initial_random(world, num_s, num_i);

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
        is_running = !is_running;//se premo un tasto la pandemia si blocca
      } else if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::White);//pulisco la finestra ogni frame

    if (is_running) {
      world = evolve(world, beta, gamma, alfa);//se is_runnig è vero la pandemia continua
    }
    display.draw(world);

    int i_counter = world.I_Number();

    if (i_counter == 0) {
      break;
    }//se gli infetti divengono zero la pandemia si ferma ed esco dal ciclo

    window.display(); //mostra a schermo il mondo nuovo disegnato prima sulla finestra
  }

  if (window.isOpen()) { //in questo if si entra dopo che i_counter=0
    int h_counter = world.Healed_Number();
    int d_counter = world.Dead_Number();
    int N_people= num_i + num_s;
    int survivors=N_people-d_counter;//sopravvissuti
    int infected=d_counter+h_counter-num_i;//numero infettati durante la pandemia
  
    std::cout<< "Il virus non può più progredire" << '\n'<<'\n';
    std::cout<< "Numero di persone totali: "<< N_people << '\n';
    std::cout<< "Numero di suscettibili iniziale: "<< num_s << '\n';
    std::cout<< "Numero di infetti iniziale: "<< num_i << '\n';
    std::cout<< "Numero di sopravvisuti: "<< survivors << '\n';
    std::cout<< "Numero di guariti: "<< h_counter << '\n';
    std::cout<< "Numero di vittime del virus: "<< d_counter << '\n';
    std::cout<< "Numero di infettati durante la pandemia: "<< infected << '\n';
    //stampa dati finali su terminale
    
    sf::Text text;//testo per rettangolo di riepilogo
    sf::Font font;//font per rettangolo di riepilogo
    if (!font.loadFromFile("include/Roboto-Regular.ttf")) {
      throw std::runtime_error("can't load Roboto font");
    }
    text.setFont(font);//font setting
  
    text.setCharacterSize(20);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Black);
    text.setPosition(6, 6);
  
    sf::Vector2f size(270, 160);//vettore bidimensionale per il rettangolo di riepilogo
    sf::RectangleShape rect(size);//rettangolo analisi dati
    rect.setFillColor(sf::Color::White);
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(3.0);
    rect.setPosition(3, 3);

    std::string counter_message =
        "Number of initial S: " + std::to_string(num_s) + '\n' +
        "Number of initial I: " + std::to_string(num_i) + '\n' +
        "Number of Healed: " + std::to_string(h_counter) + '\n' +
        "Number of Dead: " + std::to_string(d_counter) + '\n' +
        "Number of Survivors: " + std::to_string(survivors) + '\n' +
        "Number of Infected: " + std::to_string(infected);
    text.setString(counter_message);

    window.clear(sf::Color::White);
    display.draw(world);
    display.show_message("Epidemy has stopped");

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