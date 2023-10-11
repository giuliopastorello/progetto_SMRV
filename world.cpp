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
        int index=counter_r+c-1;
        assert(index>=0 && index <=m_side*m_side-1); 
        /*riporta la posizione (r,c) in notazione matriciale, 
        -1 poichè gli indici vettoriali partono da zero; li calcolo partendo da 1 e poi li riscalo*/
        return m_field[index];
    }

    void World::Set_cell(Cell cell_type,int r, int c){
        int const counter_r= m_side*(r-1); 
        int index=counter_r+c-1;
        assert(index>=0 && index <=m_side*m_side-1);
        m_field[index]=cell_type; 
    }

}