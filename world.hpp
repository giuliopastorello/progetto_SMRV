#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>

namespace GameofLife{

enum class Cell {Dead, Empty, S, I, Healed };

class World {

   int m_side; //dimensione griglia m_side x m_side
   std::vector<Cell> m_field; //vettore griglia di celle

   public:

   int side() const; //get per il side
   int S_Number() const; //get numero di suscettibili
   int I_Number() const; //get numero di infetti
   int Healed_Number() const; //get numero di rimossi (guariti)
   int Dead_Number() const; //get numero di morti


   void Set_cell(Cell const &cell_type,int r, int c); //cambia il valore della cella in posizione (r,c)
   
   Cell Get_cell(int r,int c) const;// restituisce la cella in posizione (r,c) riga colonna
   
   World(int a); //costruttore celle vuote
   
};//class world

void initial_random(World &world, int num_s, int num_i); //metodo che prende una griglia creata e genera random infetti e suscettibili

int infected_counter(World const &World,int r,int c); //conta il numero di celle infette intorno 

bool infected(int number_counter,double beta); //metodo per stabilire se contagiare
  
bool removed(double gamma); //metodo per stabilire se rimuovere

bool fatality(double alfa); //metodo per stabilire se muore

bool move_condition(World const &World,int r,int c);

World evolve(World &World, double beta, double gamma, double alfa);

void worldDisplay(World const &World);//metodo per graficare su terminale

void worldDisplayGrid(World const &World);//metodo per graficare una griglia con scacchiera

bool virus_condition(World const &World);//metodo che verifica se ha senso far proseguire il programma


}//namespace gameoflife


#endif