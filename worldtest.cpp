#include "world.hpp"
#include <iostream>
#include <stdexcept>

int main(){
   
   GameofLife::World world_test(4);
   world_test.Set_cell(GameofLife::Cell::S,2,2);
   if  (world_test.Get_cell(2,2)==GameofLife::Cell::S){
      std::cout<<"risultato esatto" << '\t';
   }
  
}
