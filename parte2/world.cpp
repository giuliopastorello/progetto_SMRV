#include "world.hpp"

namespace VirusGame{

    int World::side() const { return m_side;}

    int World::Healthy_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::Healthy);
    }

    int World::I_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::Infected);
    }

    int World::Healed_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::Healed);
    }

    int World::D_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::Dead);
    }

    World::World(int a): m_side{a}, m_field(a*a, Cell::Empty){ 
       if ( a<1 ){
         throw std::runtime_error("Invalid grid dimension");
       }
    }

    World::World(World const &world) {
      m_side = world.m_side;
      m_field = world.m_field;
    }

    Cell World::Get_cell(int r, int c) const{
        auto i = (r + m_side) % m_side;
        auto j = (c + m_side) % m_side;
        assert(i >= 0 && i < m_side && j >= 0 && j < m_side);
        auto index = i * m_side + j;
        assert(index >= 0 && index < m_side * m_side);
        return m_field[index];
    }
    /*
    griglia toroidale: vale l'effetto pacman
    questo metodo identifica le celle del vettore in una matrice m_side x m_side
    per noi la matrice è (r,c):
    (0,0) (0,1) (0,2)
    (1,0) (1,1) (1,2)
    (2,0) (2,1) (2,2)
    */

    void World::Set_cell(Cell const &cell_type, int r, int c){
        auto i = (r + m_side) % m_side;
        auto j = (c + m_side) % m_side;
        assert(i >= 0 && i < m_side && j >= 0 && j < m_side);
        auto index = i * m_side + j;
        assert(index >= 0 && index < m_side * m_side);
        m_field[index] = cell_type; 
    }

    int infected_counter(World const &World, int r, int c){
      int result = 0;
      for (int i : {-1, 0, 1}) {
        for (int j : {-1, 0, 1}) {
          if (World.Get_cell(r + i, c + j) == Cell::Infected) {
             ++result;
          }
        }
      }
      return result;
    } /*conta il numero di celle infette intorno 
    (sembra contare anche sè stessa però verrà usato solo su celle sane) */

    bool move_condition(World const &World, int r, int c){
      bool condition = false;
      if (World.Get_cell(r, c) == Cell::Dead){
        return false;
      } else {
         for (int i : {-1, 0, 1}) {
           if(condition){ break;}
           for (int j : {-1, 0, 1}) {
             if (World.Get_cell(r + i, c + j) == Cell::Empty) {
                condition = true;
                break;
             }
           }
         }
      }
      return condition;
    }//metodo che controlla se intorno ci sono celle vuote in cui spostarsi e restituisce vero nel caso
    //se la cella su cui viene chiamata è morta restituisce falso

    bool infection_condition(int number_counter, double beta){
       assert(beta >= 0 && beta <= 1 );
       std::random_device rand{};
       std::default_random_engine eng{rand()};
       std::uniform_int_distribution<int> dist{0, 100};

       int m = dist(eng);
       int prob = std::round(beta * number_counter * 20); //per beta=0.5 ogni infetto vicino +10%

       return m < prob; //se numero generato minore di prob restituisce vero -> cella infettata
    } //metodo per stabilire se contagiare

    bool removal_condition(double gamma){
       assert(gamma >= 0 && gamma <= 1);
       std::random_device rand{};
       std::default_random_engine eng{rand()};
       std::uniform_int_distribution<int> dist{0, 100};
     
       int m = dist(eng);
       int prob = std::round(gamma * 100); //qui chiaramente non contano le celle confinanti
                                          //per gamma=0.1 10% probabilità rimozione  
       return m < prob;
    }//metodo per stabilire se rimuovere

    bool death_condition(double alfa){
       assert(alfa >= 0 && alfa <= 1);
       std::random_device rand{};
       std::default_random_engine eng{rand()};
       std::uniform_int_distribution<int> dist{0, 100};
 
       int m = dist(eng);
       int prob = std::round(alfa * 100);//per alfa=0.05 5% prob di morire 
 
       return m < prob;     
    }//metodo per stabilire la morte: vero->morte

    void initial_random(World &world, int num_healthy, int num_infected){
        int const N = world.side();

        if (num_healthy < 0 || num_infected < 0) {
           throw std::runtime_error("There can be no negative number of people");
        }
        if (num_infected < 1) {
           throw std::runtime_error("The game is not interesting without infected");
        }
        if (num_healthy + num_infected > world.side() * world.side()) {
           throw std::runtime_error("There are too many people");
        }
     
        std::random_device r{};
        std::default_random_engine eng{r()};
        std::uniform_int_distribution<int> dist{1, N};

        for (int i = 0; i != num_healthy; ++i) {
           auto r = dist(eng);                                       
           auto c = dist(eng);
       
           while (world.Get_cell(r, c) != Cell::Empty) {             
            r = dist(eng);
            c = dist(eng);
           }
           world.Set_cell(Cell::Healthy, r, c); 
        }

        for (int i = 0; i != num_infected; ++i) {
           auto r = dist(eng);
           auto c = dist(eng);
       
           while (world.Get_cell(r, c) != Cell::Empty) {
             r = dist(eng);
             c = dist(eng);
           }
           world.Set_cell(Cell::Infected, r, c);
        }
    } //metodo che prende una griglia creata e genera random infetti e suscettibili

    World evolve(World const &now, double beta, double gamma, double alfa){
       if (beta < 0 || beta > 1 || gamma < 0 || gamma > 1 || alfa < 0 || alfa > 1) {
         throw std::runtime_error("Probability parameters must be between 0 and 1");
       }

       int const N = now.side();

       World next(now);//copio la griglia attuale

       std::random_device r{};
       std::default_random_engine eng{r()};
       std::uniform_int_distribution<int> dist{-1, 1};
       //numero causale tra -1 0 1 per far spostare random gli abitanti della griglia

       for(int r = 0; r != N ; ++r){
          for(int c = 0; c != N; ++c){
              bool condition = move_condition(next, r, c);
              //restituisce vero se ho una cella empty intorno
              //va fatto su next poichè la grglia cambia durante il ciclo
              //restituisce falso se la cella è morta o è bloccata
              
              int infected_around = infected_counter(now, r, c);
              //va fatto su now perchè l'infezione dipende dai contatti dello stato prima 

              if (condition == false){
                switch (now.Get_cell(r, c)) {
                  case Cell::Healthy:
                   if (infection_condition(infected_around, beta)) {
                     next.Set_cell(Cell::Infected, r, c);
                   } else {
                     next.Set_cell(Cell::Healthy, r, c);
                   }
                    break;

                  case Cell::Infected:
                    if (removal_condition(gamma)) {  
                      if(death_condition(alfa)){
                        next.Set_cell(Cell::Dead, r, c);
                      } else {
                        next.Set_cell(Cell::Healed, r, c);//se morte falso -> cura
                      }
                    } else {
                        next.Set_cell(Cell::Infected, r, c);
                    }
                    break;

                  default:
                    break;
                } //se la cella non ha dove spostarsi rimane ferma evolvendosi in base al suo stato precedente

              } else {                        
                  auto a = dist(eng);
                  auto b = dist(eng);
    
                  while (next.Get_cell(r + a, c + b) != Cell::Empty) {
                    a = dist(eng);
                    b = dist(eng);
                  }//continuo a generare una posizione random finchè non viene trovata una libera
                  //sono sicuro che esista una posizione intorno libera poichè condition è true
    
                  switch (now.Get_cell(r, c)) {
                     case Cell::Healthy:
                       if (infection_condition(infected_around, beta)) {
                         next.Set_cell(Cell::Empty, r, c);
                         next.Set_cell(Cell::Infected, r + a, c + b);
                       } else {
                         next.Set_cell(Cell::Empty, r, c);
                         next.Set_cell(Cell::Healthy, r + a, c + b);
                       }
                       break;
             
                     case Cell::Infected:
                       if (removal_condition(gamma)) {
                        if(death_condition(alfa)){
                          next.Set_cell(Cell::Empty, r, c);
                          next.Set_cell(Cell::Dead, r + a, c + b);
                        } else {
                          next.Set_cell(Cell::Empty, r, c);
                          next.Set_cell(Cell::Healed, r + a, c + b);//se fat falso viene rimossa (curata)
                        }
                       }  else {
                         next.Set_cell(Cell::Empty, r, c);
                         next.Set_cell(Cell::Infected, r + a, c + b);
                       }
                       break;
             
                     case Cell::Healed:
                         next.Set_cell(Cell::Empty, r, c);
                         next.Set_cell(Cell::Healed, r + a, c + b);
                         break;
             
                     default:
                       break;
                  }//se l'individuo può spostarsi si sposta ed evolve secondo le stesse regole 
              }
          }
        }

        assert(now.Healthy_Number() + now.I_Number() + now.Healed_Number() + now.D_Number() == 
               next.Healthy_Number() + next.I_Number() + next.Healed_Number()+ next.D_Number());
        //verifico che il numero di abitanti nella griglia sia rimasto lo stesso

        return next;
    }

    bool virus_condition(World const &world){
      bool cond = true;
      if(world.I_Number() == 0){
        cond = false;
      }
      return cond;
    }

    void worldDisplay(World const &World){
      int const N = World.side();

      std::cout << termcolor::on_bright_white << termcolor::grey
      << "VIRUS GAME" << termcolor::reset << '\n';

      for(int i = 0; i != N+2; ++i) {
        std::cout << termcolor::bright_white << "-";
      }

      std::cout << '\n';

      for(int r = 0; r != N; ++r) {
        for(int c = 0; c != N; ++c) {
          if(c == 0){
            std::cout << termcolor::bright_white << "|";
          }
          switch (World.Get_cell(r, c)) {
              case Cell::Healthy:
                std::cout << termcolor::bright_blue << "o";
                break;
         
              case Cell::Infected:
                std::cout << termcolor::bright_red << "o";
                break;
         
              case Cell::Healed:
                std::cout << termcolor::green << "x";
                break;

              case Cell::Dead:
                std::cout << termcolor::grey << "x";
                break;
                 
              case Cell::Empty:
                std::cout << " ";
                break;
        
              default:
                break;
            }

            if(c == N-1){
              std::cout << termcolor::bright_white << "|" << '\n';
            }
            
        }
      }

      for(int i = 0; i != N+2; ++i){
         std::cout << termcolor::bright_white << "-";
      }

      std::cout << '\n' << '\n';

    }//metodo stamba semplice

    void worldDisplayGrid(World const &World){
          int const N = World.side();
    
          std::cout << termcolor::on_bright_white << termcolor::grey
          << "VIRUS GAME" << termcolor::reset << '\n'; //titolo
    
          for(int r = 0; r != N; ++r){

            for(int i = 0; i != N; ++i){
               std::cout << termcolor::on_bright_white << termcolor::grey << "+-"
               << termcolor::reset;
            }
            std::cout << termcolor::on_bright_white << termcolor::grey << "+"
            << termcolor::reset << '\n';

            for(int c = 0; c != N; ++c){
              
              switch (World.Get_cell(r, c)) {
                  case Cell::Healthy:
                    std::cout << termcolor::on_bright_white << termcolor::grey << "|"
                    << termcolor::reset;
                    std::cout << termcolor::on_bright_white << termcolor::bright_blue << "o"
                    << termcolor::reset;
                    break;
             
                  case Cell::Infected:
                    std::cout << termcolor::on_bright_white << termcolor::grey << "|"
                    << termcolor::reset;
                    std::cout << termcolor::on_bright_white << termcolor::red << "o"
                    << termcolor::reset;
                    break;
             
                  case Cell::Healed:
                    std::cout << termcolor::on_bright_white << termcolor::grey << "|"
                    << termcolor::reset;
                    std::cout << termcolor::on_bright_white << termcolor::green << "x"
                    << termcolor::reset;
                    break;

                  case Cell::Dead:
                    std::cout << termcolor::on_bright_white << termcolor::grey << "|"
                    << termcolor::reset;
                    std::cout << termcolor::on_bright_white << termcolor::grey << "x"
                    << termcolor::reset;
                    break;
                     
                  case Cell::Empty:
                    std::cout << termcolor::on_bright_white << termcolor::grey << "|"
                    << termcolor::reset;
                    std::cout << termcolor::on_bright_white << " "
                    << termcolor::reset;
                    break;
            
                  default:
                    break;
                }
    
                if(c == N-1){
                  std::cout << termcolor::on_bright_white << termcolor::grey << "|"
                  << termcolor::reset << '\n';
                }               
            }
          }
    
          for(int i = 0; i != N; ++i){
              std::cout << termcolor::on_bright_white << termcolor::grey << "+-"
              << termcolor::reset;
          }
              
          std::cout << termcolor::on_bright_white << termcolor::grey << "+"
          << termcolor::reset << '\n';
    
        }//metodo stampa con griglia a scacchiera

}//namespace gameoflife