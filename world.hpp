#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>

namespace GameofLife{

enum class Cell { Empty, S, I, R };

class World {

   int m_side; //dimensione griglia m_side x m_side
   std::vector<Cell> m_field; //vettore griglia di celle

   public:

   int side() const; //get per il side
   int S_Number() const; //get numero di suscettibili
   int I_Number() const; //get numero di infetti
   int R_Number() const; //get numero di rimossi

   void Set_cell(Cell cell_type,int r, int c); //cambia il valore della cella in posizione (r,c)
   
   Cell Get_cell(int r,int c) const;// restituisce la cella in posizione (r,c) riga colonna
   
   World(int a); //costruttore celle vuote
   
};//class world

void Initial_Random_World(World World); //metodo che prende una griglia creata e genera random infetti e suscettibili

}//namespace gameoflife


#endif