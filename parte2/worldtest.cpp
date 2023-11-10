#include "world.hpp"
#include <iostream>
#include <unistd.h>
#include "../include/termcolor.hpp"


int main(){

   system("clear");

   //int i=0;
   //int N_times=25;
   int dimension = 4;
   int number_S = 3;
   int number_I = 2;
   float Beta = 0.5;
   float Gamma = 0.05;
   float alfa = 0.10;
   float time_inter = 1.5;
   int N_people=number_I + number_S;

   Virus::World world_test(dimension);

   Virus::initial_random(world_test,number_S,number_I);
   //GameofLife::worldDisplay(world_test);
   Virus::worldDisplayGrid(world_test);
   
   Virus::World next(dimension);
   
   while(Virus::virus_condition(world_test)){
      sleep(time_inter);
      system("clear");
      next=Virus::evolve(world_test,Beta,Gamma,alfa);
      //GameofLife::worldDisplay(next);
      Virus::worldDisplayGrid(next);
      world_test = next;
   }

   //analisi risultati:
   int survivors=N_people-world_test.D_Number();
   int restored=world_test.R_Number();
   int dead=world_test.D_Number();
   int infected=world_test.D_Number()+world_test.R_Number()-number_I;


   if(Virus::virus_condition(world_test)==false){
      std::cout<< "Il virus non può più progredire" << '\n'<<'\n';
      std::cout<< "Numero di persone totali: "<< N_people << '\n';
      std::cout<< "Numero di suscettibili iniziale: "<< number_S << '\n';
      std::cout<< "Numero di infetti iniziale: "<< number_I << '\n';
      std::cout<< "Numero di sopravvisuti: "<< survivors << '\n';
      std::cout<< "Numero di guariti: "<< restored << '\n';
      std::cout<< "Numero di vittime del virus: "<< dead << '\n';
      std::cout<< "Numero di infettati durante la pandemia: "<< infected << '\n';
   }


   
}
