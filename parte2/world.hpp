#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include "../include/termcolor.hpp"
#include <algorithm> //contiene metodo count per ricerca nei vettori
#include <cassert>
#include <cmath>
#include <random>
#include <iostream>

namespace VirusGame{

enum class Cell {Dead, Empty, Healthy, Infected, Healed };

class World {

   int m_side; //dimensione griglia m_side x m_side
   std::vector<Cell> m_field; //vettore griglia di celle

   public:

   int side() const; 
   int Healthy_Number() const; 
   int I_Number() const; 
   int Healed_Number() const; 
   int D_Number() const;

   void Set_cell(Cell const &cell_type, int r, int c);
   
   Cell Get_cell(int r,int c) const;
   
   World(int a); //costruttore celle vuote
   
};//class world

void initial_random(World &world, int num_healthy, int num_infected); //metodo che prende una griglia creata e genera random infetti e suscettibili

int infected_counter(World const &World, int r, int c); //conta il numero di celle infette intorno 

bool infection_condition(int number_counter, double beta); //metodo per stabilire se contagiare
  
bool removal_condition(double gamma); //metodo per stabilire se rimuovere

bool death_condition(double alfa); //metodo per stabilire se muore

bool move_condition(World const &World, int r, int c);//metodo che verifica se la cella si può spostare

World evolve(World &World, double beta, double gamma, double alfa);

void worldDisplay(World const &World);//metodo per graficare su terminale

void worldDisplayGrid(World const &World);//metodo per graficare una griglia con scacchiera

bool virus_condition(World const &World);//metodo che verifica se ha senso far proseguire il programma

}//namespace gameoflife

#endif