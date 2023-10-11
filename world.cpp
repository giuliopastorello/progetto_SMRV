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
        int const counter_r= m_side*(r-1); //ritorna la posizione (r-1,m_side) in notazione matriciale (+1)
        assert(counter_r>=0 && counter_r<=(m_side*m_side-1) && c>=1 && c<=m_side);
        int index=counter_r+c-1;
        assert(index>=0 && index <=m_side*m_side-1); 
        /*riporta la posizione (r,c) in notazione matriciale, 
        -1 poichè gli indici vettoriali partono da zero; li calcolo partendo da 1 e poi li riscalo*/
        return m_field[index];
    }

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
    } //conta il numero di celle infette intorno 

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

    void Initial_Random_World(World &World); //metodo che prende una griglia creata e genera random infetti e suscettibili

    World evolve(World &corrente,double beta,double gamma);
    




}//namespace gameoflife