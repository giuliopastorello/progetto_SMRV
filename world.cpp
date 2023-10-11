#include "world.hpp"

#include "stdexcept"
#include <algorithm> //contiene metodo count per ricerca nei vettori
#include <cassert>
#include <cmath>
#include <random>

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


    int Cell_counter(World const &World,int r,int c){
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

    World evolve(World &corrente,double beta,double gamma);
    




}//namespace gameoflife