#include "world.hpp"

namespace Virus {

    int World::get_side() const { return m_side;}

    int World::S_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::S);
    }

    int World::I_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::I);
    }

    int World::R_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::R);
    }

    int World::D_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::Dead);
    }

    World::World(int a): m_side{a}, m_field(a*a, Cell::Empty) { 
       if ( a<1 ){
         throw std::runtime_error("Invalid grid dimension");
       }
    }

    World::World(World const &world) {
      m_side = world.m_side;
      m_field = world.m_field;
    }

    Cell World::Get_cell(int r, int c) const {
        auto i = (r + m_side) % m_side;
        auto j = (c + m_side) % m_side;
        assert(i >= 0 && i < m_side && j >= 0 && j < m_side);
        auto index = i * m_side + j;
        assert(index >= 0 && index < m_side * m_side);
        return m_field[index];
    }

    void World::Set_cell(Cell const &cell_type, int r, int c) {
        auto i = (r + m_side) % m_side;
        auto j = (c + m_side) % m_side;
        assert(i >= 0 && i < m_side && j >= 0 && j < m_side);
        auto index = i * m_side + j;
        assert(index >= 0 && index < m_side * m_side);
        m_field[index] = cell_type; 
    }

    int infected_counter(World const &World, int r, int c) {
      int result = 0;
      for (int i : {-1, 0, 1}) {
        for (int j : {-1, 0, 1}) {
          if (World.Get_cell(r + i, c + j) == Cell::I) {
             ++result;
          }
        }
      }
      return result;
    }

    bool move_condition(World const &World, int r, int c) {
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
    }

    bool infection_condition(int number_counter, double beta) {
       assert(beta >= 0 && beta <= 1 );
       std::random_device rand{};
       std::default_random_engine eng{rand()};
       std::uniform_int_distribution<int> dist{0, 100};

       int m = dist(eng);
       int prob = std::round(beta * number_counter * 20);

       return m < prob;
    }

    bool removal_condition(double gamma) {
       assert(gamma >= 0 && gamma <= 1);
       std::random_device rand{};
       std::default_random_engine eng{rand()};
       std::uniform_int_distribution<int> dist{0, 100};
     
       int m = dist(eng);
       int prob = std::round(gamma * 100);
                                            
       return m < prob;
    }

    bool death_condition(double alfa) {
       assert(alfa >= 0 && alfa <= 1);
       std::random_device rand{};
       std::default_random_engine eng{rand()};
       std::uniform_int_distribution<int> dist{0, 100};
 
       int m = dist(eng);
       int prob = std::round(alfa * 100); 
 
       return m < prob;     
    }

    void initial_random(World &world, int num_healthy, int num_infected) {
        int const N = world.get_side();

        if (num_healthy < 0 || num_infected < 0) {
           throw std::runtime_error("There can be no negative number of people");
        }
        if (num_infected < 1) {
           throw std::runtime_error("The game is not interesting without infected");
        }
        if (num_healthy + num_infected > world.get_side() * world.get_side()) {
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
           world.Set_cell(Cell::S, r, c); 
        }

        for (int i = 0; i != num_infected; ++i) {
           auto r = dist(eng);
           auto c = dist(eng);
       
           while (world.Get_cell(r, c) != Cell::Empty) {
             r = dist(eng);
             c = dist(eng);
           }
           world.Set_cell(Cell::I, r, c);
        }
    }

    World evolve(World const &now, double beta, double gamma, double alfa) {

       if (beta < 0 || beta > 1 || gamma < 0 || gamma > 1 || alfa < 0 || alfa > 1) {
         throw std::runtime_error("Probability parameters must be between 0 and 1");
       }

       int const N = now.get_side();

       World next(now);//copy

       std::random_device r{};
       std::default_random_engine eng{r()};
       std::uniform_int_distribution<int> dist{-1, 1};

       for(int r = 0; r != N ; ++r){
          for(int c = 0; c != N; ++c){
              bool condition = move_condition(next, r, c);
              //on next as the grid is changing during the cicles
              
              int infected_around = infected_counter(now, r, c);

              if (condition == false){
                switch (now.Get_cell(r, c)) {
                  case Cell::S:
                   if (infection_condition(infected_around, beta)) {
                     next.Set_cell(Cell::I, r, c);
                   } else {
                     next.Set_cell(Cell::S, r, c);
                   }
                    break;

                  case Cell::I:
                    if (removal_condition(gamma)) {  
                      if(death_condition(alfa)){
                        next.Set_cell(Cell::Dead, r, c);
                      } else {
                        next.Set_cell(Cell::R, r, c);
                      }
                    } else {
                        next.Set_cell(Cell::I, r, c);
                    }
                    break;

                  default:
                    break;
                } 

              } else {                        
                  auto a = dist(eng);
                  auto b = dist(eng);
    
                  while (next.Get_cell(r + a, c + b) != Cell::Empty) {
                    a = dist(eng);
                    b = dist(eng);
                  }
    
                  switch (now.Get_cell(r, c)) {
                     case Cell::S:
                       if (infection_condition(infected_around, beta)) {
                         next.Set_cell(Cell::Empty, r, c);
                         next.Set_cell(Cell::I, r + a, c + b);
                       } else {
                         next.Set_cell(Cell::Empty, r, c);
                         next.Set_cell(Cell::S, r + a, c + b);
                       }
                       break;
             
                     case Cell::I:
                       if (removal_condition(gamma)) {
                        if(death_condition(alfa)){
                          next.Set_cell(Cell::Empty, r, c);
                          next.Set_cell(Cell::Dead, r + a, c + b);
                        } else {
                          next.Set_cell(Cell::Empty, r, c);
                          next.Set_cell(Cell::R, r + a, c + b);
                        }
                       }  else {
                         next.Set_cell(Cell::Empty, r, c);
                         next.Set_cell(Cell::I, r + a, c + b);
                       }
                       break;
             
                     case Cell::R:
                         next.Set_cell(Cell::Empty, r, c);
                         next.Set_cell(Cell::R, r + a, c + b);
                         break;
             
                     default:
                       break;
                  } 
              }
          }
        }

        assert(now.S_Number() + now.I_Number() + now.R_Number() + now.D_Number() == 
               next.S_Number() + next.I_Number() + next.R_Number()+ next.D_Number());

        return next;
    }

    bool virus_condition(World const &world) {
      bool cond = true;
      if(world.I_Number() == 0){
        cond = false;
      }
      return cond;
    }

    void worldDisplay(World const &World) {
      int const N = World.get_side();

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
              case Cell::S:
                std::cout << termcolor::bright_blue << "o";
                break;
              case Cell::I:
                std::cout << termcolor::bright_red << "o";
                break;
              case Cell::R:
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
    }

    void worldDisplayGrid(World const &World) {
          int const N = World.get_side();
    
          std::cout << termcolor::on_bright_white << termcolor::grey
          << "VIRUS GAME" << termcolor::reset << '\n';
    
          for(int r = 0; r != N; ++r) {
            for(int i = 0; i != N; ++i) {
               std::cout << termcolor::on_bright_white << termcolor::grey << "+-"
               << termcolor::reset;
            }

            std::cout << termcolor::on_bright_white << termcolor::grey << "+"
            << termcolor::reset << '\n';

            for(int c = 0; c != N; ++c) {              
              switch (World.Get_cell(r, c)) {
                  case Cell::S:
                    std::cout << termcolor::on_bright_white << termcolor::grey << "|"
                    << termcolor::reset;
                    std::cout << termcolor::on_bright_white << termcolor::bright_blue << "o"
                    << termcolor::reset;
                    break;
             
                  case Cell::I:
                    std::cout << termcolor::on_bright_white << termcolor::grey << "|"
                    << termcolor::reset;
                    std::cout << termcolor::on_bright_white << termcolor::red << "o"
                    << termcolor::reset;
                    break;
             
                  case Cell::R:
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
        }

}//virus 