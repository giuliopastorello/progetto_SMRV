#include "world.hpp"
#include <iostream>
#include <stdexcept>

int main(){
   
   GameofLife::World world_test(4);
   world_test.Set_cell(GameofLife::Cell::S,1,1);
   world_test.Set_cell(GameofLife::Cell::I,1,2);
   world_test.Set_cell(GameofLife::Cell::I,2,1);
   world_test.Set_cell(GameofLife::Cell::S,3,4);
   world_test.Set_cell(GameofLife::Cell::S,2,2);
   world_test.Set_cell(GameofLife::Cell::I,3,3);
   world_test.Set_cell(GameofLife::Cell::S,4,4);
    
   std::cout<<GameofLife::infected_counter(world_test,1,1);
   std::cout<<'\n';

   /*GameofLife::worldDisplay(world_test);

   GameofLife::World next(4);
   next=GameofLife::evolve(world_test,0.2,0.2);
   GameofLife::worldDisplay(next);
   */
  
}
