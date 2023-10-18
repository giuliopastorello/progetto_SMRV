#include "world.hpp"

#include "termcolor.hpp"
#include <algorithm> //contiene metodo count per ricerca nei vettori
#include <cassert>
#include <cmath>
#include <random>
#include <iostream>


namespace GameofLife{

    int World::side() const {return m_side;}

    int World::S_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::S);
    }

    int World::I_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::I);
    }

    int World::R_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell::R);
    }

    World::World(int a):m_side{a}, m_field(a*a,Cell::Empty){ //inizializza a*a celle vuote
       if (a<1){
         throw std::runtime_error("dimensione non valida per la griglia");
       }
    }


    Cell World::Get_cell(int r,int c) const{
        auto const i = (r + m_side) % m_side;
        auto const j = (c + m_side) % m_side;
        assert(i >= 0 && i < m_side && j >= 0 && j < m_side);
        auto const index = i * m_side + j;
        assert(index>=0 && index<=m_side*m_side-1);
        return m_field[index];
    }
    /*
    griglia toroidale:vale l'effetto pacman
    questo metodo identifica le celle del vettore in una matrice m_side x m_side
    per noi la matrice è (r,c):
    (0,0) (0,1) (0,2)
    (1,0) (1,1) (1,2)
    (2,0) (2,1) (2,2)
    */

    void World::Set_cell(Cell const &cell_type,int r, int c){
        auto const i = (r + m_side) % m_side;
        auto const j = (c + m_side) % m_side;
        assert(i >= 0 && i < m_side && j >= 0 && j < m_side);
        auto const index = i * m_side + j;
        assert(index>=0 && index<=m_side*m_side-1);
        m_field[index]=cell_type; 
    }


    int infected_counter(World &World,int r,int c){
      int result=0;
      for (int i : {-1, 0, 1}) {
        for (int j : {-1, 0, 1}) {
          if (World.Get_cell(r + i, c + j) == Cell::I) {
             ++result;
          }
        }
      }
      return result;
    } /*conta il numero di celle infette intorno 
    (sembra contare anche sè stessa però verrà usato solo su celle S) */

    bool move_condition(World const &World,int r,int c){
      bool condition=false;
      for (int i : {-1, 0, 1}) {
        for (int j : {-1, 0, 1}) {
          if (World.Get_cell(r + i, c + j) == Cell::Empty) {
             condition=true;
          }
        }
      }
      return condition;
    }//metodo che controlla se intorno ci sono celle vuote in cui spostarsi e restituisce vero nel caso


    bool infected(int number_counter,double beta){
       assert(beta>=0 && beta<=1);
       std::random_device rand{};
       std::default_random_engine eng{rand()};
       std::uniform_int_distribution<int> dist{0, 100};

       int m = dist(eng);
       int prob = std::round(beta * number_counter * 10);

       return m < prob; //se numero generato minore di prob restituisce vero -> cella infettata

    } //metodo per stabilire se contagiare

    bool removed(double gamma){
     assert(gamma >= 0 && gamma <= 1);

     std::random_device rand{};
     std::default_random_engine eng{rand()};
     std::uniform_int_distribution<int> dist{0, 100};
   
     int m = dist(eng);
     int prob = std::round(gamma * 100); //qui chiaramente non contano le celle confinanti
   
     bool result = (m < prob);
   
     return result;

    } //metodo per stabilire se rimuovere

    void initial_random(World &world, int num_s, int num_i){
        int const N = world.side();

        if (num_s < 0 || num_i < 0) {
           throw std::runtime_error("non puoi mettere numeri negativi di persone");
        }
        if (num_s + num_i >=world.side()*world.side()) {
           throw std::runtime_error("la griglia non può essere tutta piena");
        } 
        
        /*else if (static_cast<double>(num_s + num_i) / static_cast<double>(N * N) >0.3) {
           throw std::runtime_error("Overpopulation: more than 30 percent of the grid populated");
          }   problema sovrappopolazione gianca non affrontato ancora */

        std::random_device r{};
        std::default_random_engine eng{r()};
        std::uniform_int_distribution<int> dist{1, N};

        for (int i = 0; i != num_s; ++i) {
           auto r = dist(eng); //vado a selezionare random una riga tra 1 e side (num righe e colonne)
           auto c = dist(eng);
       
           while (world.Get_cell(r, c) != Cell::Empty) { //se trovo una cella già occupata rigenero casualmente
            r = dist(eng);
            c = dist(eng);
           }

           world.Set_cell(Cell::S,r, c); //set della cella random con S
        }

        for (int i = 0; i != num_i; ++i) {
           auto r = dist(eng);
           auto c = dist(eng);
       
           while (world.Get_cell(r, c) != Cell::Empty) {
             r = dist(eng);
             c = dist(eng);
           }
           world.Set_cell(Cell::I,r, c);
        }
    } //metodo che prende una griglia creata e genera random infetti e suscettibili

    World evolve(World &corrente,double beta,double gamma){
       if (beta < 0 || beta > 1 || gamma < 0 || gamma > 1) {
         throw std::runtime_error("Probability parameters must be between 0 and 1");
       }

       int const N = corrente.side();

       World next{corrente};

       std::random_device r{};
       std::default_random_engine eng{r()};
       std::uniform_int_distribution<int> dist{-1, 1};
       //numero causale tra -1 0 1 per far spostare random gli abitanti della griglia

       for(int r=0;r!=N;r++){
          for(int c=0;c!=N;c++){
              bool condition=move_condition(next,r,c);
              //restituisce vero se ho una cella empty intorno
              //va fatto su next poichè la grglia cambia durante il ciclo

              int infected_around = infected_counter(corrente, r, c);
              //va fatto su corrente perchè l'infezione dipende dai contatti dello stato prima 

              if (condition==false){
                switch (corrente.Get_cell(r, c)) {
                  case Cell::S:
                   if (infected(infected_around, beta)) {
                     next.Set_cell(Cell::I,r, c);
                   } else {
                     next.Set_cell(Cell::S,r, c);
                   }
                    break;

                  case Cell::I:
                   if (removed(gamma)) {
                     next.Set_cell(Cell::R,r, c);
                   } else {
                     next.Set_cell(Cell::I,r , c );
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
    
                  switch (corrente.Get_cell(r, c)) {
                     case Cell::S:
                       if (infected(infected_around, beta)) {
                         next.Set_cell(Cell::Empty,r,c);
                         next.Set_cell(Cell::I,r + a, c + b);
                       } else {
                         next.Set_cell(Cell::Empty,r,c);
                         next.Set_cell(Cell::S,r + a, c + b);
                       }
                       break;
             
                     case Cell::I:
                       if (removed(gamma)) {
                         next.Set_cell(Cell::Empty,r,c);
                         next.Set_cell(Cell::R,r + a, c + b);
                       } else {
                         next.Set_cell(Cell::Empty,r,c);
                         next.Set_cell(Cell::I,r + a, c + b);
                       }
                       break;
             
                     case Cell::R:
                         next.Set_cell(Cell::Empty,r,c);
                         next.Set_cell(Cell::R,r + a, c + b);
                         break;
             
                     default:
                       break;
                  }//se l'individuo può spostarsi si sposta ed evolve secondo le stesse regole 
              }
          }
        }

        assert(corrente.S_Number() + corrente.I_Number() + corrente.R_Number() == 
               next.S_Number() + next.I_Number() + next.R_Number());
        //verifico che il numero di abitanti nella griglia sia rimasto lo stesso

        return next;

    }

    bool virus_condition(World &world){
      bool cond=true;
      if(world.I_Number()==0 || world.S_Number()==0){
        cond=false;
      }
      return cond;
    }

    void worldDisplay(World &World){
      int const N=World.side();

      std::cout<<termcolor::on_bright_white<<termcolor::grey
      <<"VIRUS GAME"<<termcolor::reset<<'\n';

      for(int i=0;i!=N+2;i++){
        std::cout<< termcolor::bright_white <<"-";
      }

      std::cout<<'\n';

      for(int r=0;r!=N;r++){
        for(int c=0;c!=N;c++){
          if(c==0){
            std::cout<< termcolor::bright_white <<"|";
          }
          switch (World.Get_cell(r, c)) {
              case Cell::S:
                std::cout<< termcolor::white <<"o";
                break;
         
              case Cell::I:
                std::cout<< termcolor::bright_red <<"o";
                break;
         
              case Cell::R:
                std::cout<< termcolor::bright_blue <<"x";
                break;
                 
              case Cell::Empty:
                std::cout<<" ";
                break;
        
              default:
                break;
            }

            if(c==N-1){
              std::cout<< termcolor::bright_white <<"|"<<'\n';
            }
            
        }
      }

      for(int i=0;i!=N+2;i++){
         std::cout<< termcolor::bright_white <<"-";
      }


      std::cout<<'\n';

    }


    void worldDisplayGrid(World &World){
          int const N=World.side();
    
          std::cout<<termcolor::on_bright_white<<termcolor::grey
          <<"VIRUS GAME"<<termcolor::reset<<'\n';//titolo
    
           
          for(int r=0;r!=N;r++){

            for(int i=0;i!=N;i++){
               std::cout<< termcolor::on_bright_white << termcolor::grey <<"+-"
               <<termcolor::reset;
            }
            std::cout<< termcolor::on_bright_white << termcolor::grey <<"+"
            <<termcolor::reset<<'\n';

            for(int c=0;c!=N;c++){
              
              switch (World.Get_cell(r, c)) {
                  case Cell::S:
                    std::cout<< termcolor::on_bright_white << termcolor::grey <<"|"
                    <<termcolor::reset;
                    std::cout<< termcolor::on_bright_white << termcolor::grey <<"o"
                    <<termcolor::reset;
                    break;
             
                  case Cell::I:
                    std::cout<< termcolor::on_bright_white << termcolor::grey <<"|"
                    <<termcolor::reset;
                    std::cout<< termcolor::on_bright_white << termcolor::red <<"o"
                    <<termcolor::reset;
                    break;
             
                  case Cell::R:
                    std::cout<< termcolor::on_bright_white << termcolor::grey <<"|"
                    <<termcolor::reset;
                    std::cout<< termcolor::on_bright_white << termcolor::blue <<"x"
                    <<termcolor::reset;
                    break;
                     
                  case Cell::Empty:
                    std::cout<< termcolor::on_bright_white << termcolor::grey <<"|"
                    <<termcolor::reset;
                    std::cout<< termcolor::on_bright_white <<" "
                    <<termcolor::reset;
                    break;
            
                  default:
                    break;
                }
    
                if(c==N-1){
                  std::cout<< termcolor::on_bright_white << termcolor::grey <<"|"
                  <<termcolor::reset<<'\n';
                }
                
            }
          }
    
          for(int i=0;i!=N;i++){
              std::cout<< termcolor::on_bright_white << termcolor::grey <<"+-"
              <<termcolor::reset;
          }
              
          std::cout<< termcolor::on_bright_white << termcolor::grey <<"+"
          <<termcolor::reset<<'\n';
    
        }
    


}//namespace gameoflife