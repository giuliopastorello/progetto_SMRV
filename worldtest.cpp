#include "world.hpp"
#include <iostream>
#include <stdexcept>

int main(){
   
   GameofLife::World world_test(4);
   world_test.Set_cell(GameofLife::Cell::S,1,1);
   world_test.Set_cell(GameofLife::Cell::I,1,2);
   world_test.Set_cell(GameofLife::Cell::I,2,1);
    
   std::cout<<GameofLife::infected_counter(world_test,2,2);
  
}
