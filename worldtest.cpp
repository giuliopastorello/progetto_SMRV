#include "world.hpp"
#include <iostream>
#include <unistd.h>


int main(){

   system("clear");
   
   GameofLife::World world_test1(10);

   GameofLife::initial_random(world_test1,30,10);
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

   if(GameofLife::virus_condition(world_test1)==false){
      std::cout<< "il virus non può più progredire" << '\n';
   }
   
}
