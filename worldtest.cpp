#include "world.hpp"
#include <iostream>
#include <unistd.h>
#include "include/termcolor.hpp"


int main(){

   system("clear");

   //int i=0;
   //int N_times=25;
   int dimension=4;
   int number_S=3;
   int number_I=2;
   float Beta=0.5;//contagio (con 0.5-> +5% per ogni vicino infetto)
   float Gamma=0.05;//rimozione (con 0.1 -> 10% probabilità rimozione -> o morto o guarito)
   float alfa=0.10;//fatalità (con 0.1 -> 10% probabilità morte 90% probabilità guarigione)
   float time_inter=1.5;//secondi / tempo intercorso fra un frame e l'altro
   int N_people=number_I + number_S;//numero persone totali sulla griglia

   GameofLife::World world_test(dimension);

   GameofLife::initial_random(world_test,number_S,number_I);
   //GameofLife::worldDisplay(world_test);
   GameofLife::worldDisplayGrid(world_test);
   
   GameofLife::World next(dimension);
   
   while(GameofLife::virus_condition(world_test)){
      sleep(time_inter);
      system("clear");
      next=GameofLife::evolve(world_test,Beta,Gamma,alfa);
      //GameofLife::worldDisplay(next);
      GameofLife::worldDisplayGrid(next);
      world_test=next;
   }

   //analisi risultati:
   int survivors=N_people-world_test.Dead_Number();//sopravvissuti
   int restored=world_test.Healed_Number();//guariti
   int dead=world_test.Dead_Number();//morti
   int infected=world_test.Dead_Number()+world_test.Healed_Number()-number_I;//numero infettati durante la pandemia


   if(GameofLife::virus_condition(world_test)==false){
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
