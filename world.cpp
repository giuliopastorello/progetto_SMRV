#include "world.hpp"

#include "stdexcept"
#include <algorithm> //contiene metodo count per ricerca nei vettori
#include <cassert>
#include <cmath>
#include <random>
namespace GameofLife{

    int World::side() const{return m_side;}

    int World::S_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell_Type::S);
    }

    int World::I_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell_Type::I);
    }

    int World::R_Number() const {
        return std::count(m_field.begin(), m_field.end(), Cell_Type::R);
    }

    World::World(int a):m_side{a}, m_field(a*a,Cell_Type::Empty){ //inizializza a*a celle vuote
       if (a<1){
         throw std::runtime_error("Invalid grid dimension");
       }
    }

    Cell_Type Get_cell(int r,int c){
        //bisogna convertire gli indici matriciali r,c con gli indici del vettore Field
        int const i=r;
        int const j=c;
    }

}