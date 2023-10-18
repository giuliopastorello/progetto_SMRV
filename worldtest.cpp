#include "world.hpp"
#include <iostream>
#include <unistd.h>
#include "termcolor.hpp"


int main(){

   system("clear");

   int i=0;
   int N_times=25;
   int dimension=15;
   int number_S=60;
   int number_I=25;
   float Beta=0.5;
   float Gamma=0.10;
   float time_inter=1.5;//secondi / tempo intercorso fra un frame e l'altro

   GameofLife::World world_test(dimension);

   GameofLife::initial_random(world_test,number_S,number_I);
   //GameofLife::worldDisplay(world_test);
   GameofLife::worldDisplayGrid(world_test);
   
   GameofLife::World next(dimension);
   
   while(i<N_times && GameofLife::virus_condition(world_test)){
      sleep(time_inter);
      system("clear");
      next=GameofLife::evolve(world_test,Beta,Gamma);
      //GameofLife::worldDisplay(next);
      GameofLife::worldDisplayGrid(next);
      world_test=next;
      i++;
   }

   if(GameofLife::virus_condition(world_test)==false){
      std::cout<< "il virus non può più progredire" << '\n';
   }


   
}
