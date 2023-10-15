#include "world.hpp"
#include <iostream>
#include <stdexcept>

int main(){
   
   GameofLife::World world_test(4);
   world_test.Set_cell(GameofLife::Cell::S,0,0);
   world_test.Set_cell(GameofLife::Cell::I,1,1);
   world_test.Set_cell(GameofLife::Cell::S,0,1);
   world_test.Set_cell(GameofLife::Cell::I,3,3);

   std::cout<<GameofLife::infected_counter(world_test,0,0);
   std::cout<<'\n';

   GameofLife::worldDisplay(world_test);

   GameofLife::World next1(4);
   next1=GameofLife::evolve(world_test,0.4,0.2);
   GameofLife::worldDisplay(next1);

   GameofLife::World next2(4);
   next2=GameofLife::evolve(next1,0.4,0.2);
   GameofLife::worldDisplay(next2);
   
  
}
