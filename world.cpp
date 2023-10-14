#include "world.hpp"

#include "stdexcept"
#include <algorithm> //contiene metodo count per ricerca nei vettori
#include <cassert>
#include <cmath>
#include <random>
#include <iostream>


namespace GameofLife{

    int World::side() const{return m_side;}

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
         throw std::runtime_error("Invalid grid dimension");
       }
    }

    Cell World::Get_cell(int r,int c) const{
        //bisogna convertire gli indici matriciali r,c nell' indice del vettore Field
        int const counter_r= m_side*(r-1); //ritorna la posizione (r-1,m_side) come indice vettoriale (+1)
        assert(counter_r>=0 && counter_r<=(m_side*m_side-1) && c>=1 && c<=m_side);
        int index=counter_r+c-1;
        assert(index>=0 && index <=m_side*m_side-1); 
        /*riporta la posizione (r,c) in indice vettoriale, 
        -1 poichè gli indici vettoriali partono da zero; li calcolo partendo da 1 e poi li riscalo*/
        return m_field[index];
    }
    /*
    questo metodo identifica le celle del vettore in una matrice m_side x m_side
    per noi la matrice è (r,c):
    (1,1) (1,2) (1,3)
    (2,1) (2,2) (2,3)
    (3,1) (3,2) (3,1)
    */

    void World::Set_cell(Cell const &cell_type,int r, int c){
        int const counter_r= m_side*(r-1); 
        assert(counter_r>=0 && counter_r<=(m_side*m_side-1) && c>=1 && c<=m_side);
        int index=counter_r+c-1;
        assert(index>=0 && index <=m_side*m_side-1);
        m_field[index]=cell_type; 
    }


    int infected_counter(World const &World,int r,int c){
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
           throw std::runtime_error("Can't have negative number of people");
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

       for(int r=1;r!=(N+1);r++){
          for(int c=1;c!=(N+1);c++){
              bool condition=move_condition(corrente,r,c);

              if (condition==false){
                continue;
              }//se la cella non si può muovere rimane lì e si passa alla prossima iterazione
              //non può mutare tale cella per ora          
              
              auto a = dist(eng);
              auto b = dist(eng);

              while (r+a>=1 && r+a<=N && c+b>=1 && c+b<=N && next.Get_cell(r + a, c + b) != Cell::Empty) {
                a = dist(eng);
                b = dist(eng);
              }//continuo a generare una posizione random finchè non viene trovata una libera
              //da analizzare se possono crearsi loop infiniti
          
              int const infected_around = infected_counter(corrente, r, c);

              switch (corrente.Get_cell(r, c)) {
                 case Cell::S:
                   if (infected(infected_around, beta)) {
                     next.Set_cell(Cell::I,r + a, c + b);
                   } else {
                     next.Set_cell(Cell::S,r + a, c + b);
                   }
                   break;
         
                 case Cell::I:
                   if (removed(gamma)) {
                     next.Set_cell(Cell::R,r + a, c + b);
                   } else {
                     next.Set_cell(Cell::I,r + a, c + b);
                   }
                   break;
         
                 case Cell::R:
                   next.Set_cell(Cell::R,r + a, c + b);
                   break;
         
                 default:
                   break;
              }
          
          }
        }

        assert(corrente.S_Number() + corrente.I_Number() + corrente.R_Number() == 
               next.S_Number() + next.I_Number() + next.R_Number());
        //verifico che il numero di abitanti nella griglia sia rimasto lo stesso

        return next;

    }

    void worldDisplay(World &World){
      int const N=World.side();

      for(int i=0;i<=(N+1);i++){
        std::cout<<"+";
      }

      std::cout<<'\n';

      for(int r=1;r!=(N+1);r++){
        for(int c=1;c!=(N+1);c++){
          if(c==1){
            std::cout<<"+";
          }
          switch (World.Get_cell(r, c)) {
              case Cell::S:
                std::cout<<"o";
                break;
         
              case Cell::I:
                std::cout<<"0";
                break;
         
              case Cell::R:
                std::cout<<"x";
                break;
                 
              case Cell::Empty:
                std::cout<<" ";
                break;
        
              default:
                break;
            }
            if(c==N){
              std::cout<<"+"<<'\n';
            }
        }
      }

      for(int j=0;j<=(N+1);j++){
        std::cout<<"+";
      }

      std::cout<<'\n';

    }


    


}//namespace gameoflife