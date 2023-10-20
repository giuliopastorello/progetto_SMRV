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
  double gamma = 0.05;//100*gamma
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

    int s_counter = world.S_Number();
    int i_counter = world.I_Number();
    int r_counter = world.Healed_Number();
    int d_counter = world.Dead_Number();

    if (i_counter == 0) {
      int N_people= num_i + num_s;
      int survivors=N_people-d_counter;//sopravvissuti
      int restored=r_counter;//guariti
      int dead=d_counter;//morti
      int infected=d_counter+r_counter-num_i;//numero infettati durante la pandemia
   
      std::cout<< "Il virus non può più progredire" << '\n'<<'\n';
      std::cout<< "Numero di persone totali: "<< N_people << '\n';
      std::cout<< "Numero di suscettibili iniziale: "<< num_s << '\n';
      std::cout<< "Numero di infetti iniziale: "<< num_i << '\n';
      std::cout<< "Numero di sopravvisuti: "<< survivors << '\n';
      std::cout<< "Numero di guariti: "<< restored << '\n';
      std::cout<< "Numero di vittime del virus: "<< dead << '\n';
      std::cout<< "Numero di infettati durante la pandemia: "<< infected << '\n';
     //per ora stampo su terminale, si potrebbe stampare su una finestra secondaria

      break;
    }//se gli infetti divengono zero la pandemia si ferma ed esco dal ciclo

    window.display(); //mostra a schermo il mondo nuovo disegnato prima sulla finestra
  }

  if (window.isOpen()) {
    window.clear(sf::Color::White);
    display.draw(world);
    display.show_message("Epidemy has stopped");
    //come ultima operazione quando i_counter==0 disegno e mostro a schermo l'ultima griglia
       
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