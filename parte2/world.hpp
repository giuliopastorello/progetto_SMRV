#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include "../include/termcolor.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <random>
#include <iostream>

namespace Virus{

enum class Cell {Dead, Empty, S, I, R };

class World {

   int m_side; //grid side
   std::vector<Cell> m_field; //grid vector

   public:

   int get_side() const;
   int S_Number() const; 
   int I_Number() const; 
   int R_Number() const; 
   int D_Number() const;

   void Set_cell(Cell const &cell_type, int r, int c);
   
   Cell Get_cell(int r,int c) const;
   
   World(int a);
   World(World const &world);
   World &operator=(World const &other) = default;
   
};//class world

void initial_random(World &world, int num_healthy, int num_infected); 

int infected_counter(World const &World, int r, int c); 

bool infection_condition(int number_counter, double beta); 
  
bool removal_condition(double gamma);

bool death_condition(double alfa);

bool move_condition(World const &World, int r, int c);

World evolve(World const &World, double beta, double gamma, double alfa);

void worldDisplay(World const &World);

void worldDisplayGrid(World const &World);

bool virus_condition(World const &World);

}//namespace Virus

#endif