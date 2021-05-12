#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "infection.hpp"

TEST_CASE("testing orbit") {
    int const N=4459000; //abitanti emilia romagna
    int const M_t0=12321;
    int const V_t0=600437;
    int const R_t0=320000;
    int const S_t0= N - M_t0 - V_t0 - R_t0;
    State init {S_t0 , M_t0, R_t0 , V_t0};

    Infection prova{100, init, N};
    


};