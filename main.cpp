#include "infection.hpp"
#include <iostream>



int main() {

    int const people=4459000; //abitanti emilia romagna
    int const M_t0=12321;
    int const V_t0=600437;
    int const R_t0=320000;
    int const S_t0= people - M_t0 - V_t0 - R_t0;
    State init {S_t0 , M_t0, R_t0 , V_t0};
    Infection Emilia_Romagna1{100, init, people};
    Emilia_Romagna1.SMRV_evolve();
    Emilia_Romagna1.tabulate(std::cout);

    std::cout << "analisi malati giorno start: " << Emilia_Romagna1.get_state(0).M;
    

}