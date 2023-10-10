#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>

namespace GameofLife{

enum class Cell_Type { Empty, S, I, R };

class World {

   int m_side; //dimensione griglia m_side x m_side
   std::vector<Cell_Type> m_field; //vettore griglia di celle

   public:

   int side() const; //get per il side
   int S_Number() const; //get numero di suscettibili
   int I_Number() const; //get numero di infetti
   int R_Number() const; //get numero di rimossi
   
   Cell_Type Get_cell(int r,int c);// restituisce la cella in posizione (r,c) riga colonna
   
   World(int a); //costruttore celle vuote
   
   void Initial_Random_World(World &World); //metodo che prende una griglia creata e genera random infetti e suscettibili
   
   

};

}


#endif