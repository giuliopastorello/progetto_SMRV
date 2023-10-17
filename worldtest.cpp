#include "world.hpp"
#include <iostream>
#include <unistd.h>


int main(){

   system("clear");
   
   GameofLife::World world_test1(10);

   GameofLife::initial_random(world_test1,40,15);
   GameofLife::worldDisplay(world_test1);

   int a=0;
   GameofLife::World next(5);
   
   while(a<20 && GameofLife::virus_condition(world_test1)){
      sleep(1.5);
      system("clear");
      next=GameofLife::evolve(world_test1,0.5,0.10);
      GameofLife::worldDisplay(next);
      world_test1=next;
      a++;
   }
   /*
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
   
   */
}
