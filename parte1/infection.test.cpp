#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../include/doctest.h"
#include "infection.hpp"

TEST_CASE("testing virus") {
    using namespace epidemic;

    SUBCASE("testing constructor errors") {
       int const people = 4459000;
       int const M_t0 = 12321;
       int const V_t0 = 600437;
       int const R_t0 = 320000;
       int const S_t0 = people - M_t0 - V_t0 - R_t0;

       int const testdays = 1;
       int const no_one = 0;
       int const days = 100;

       State initgood{S_t0, M_t0, R_t0, V_t0};
       State initbad1{S_t0, no_one, R_t0, V_t0};
       State initbad2{no_one, M_t0, R_t0, V_t0};

       CHECK_THROWS(Infection{testdays, initgood});
       CHECK_THROWS(Infection{days, initbad1});
       CHECK_THROWS(Infection{days, initbad2});
    }

    SUBCASE("testing euler/RK4 errors") {
      int const people = 4459000;
      int const M_t0 = 12321;
      int const V_t0 = 600437;
      int const R_t0 = 320000;
      int const S_t0 = people - M_t0 - V_t0 - R_t0;
      double beta = 0.5; 
      double gamma = 0.3;
      int no_vax = 118292;   
      double vel_vax = 0.035;  
      double eff_vax = 0.600;
      int const days = 100;

      State initgood{S_t0, M_t0, R_t0, V_t0};
      Infection test(days, initgood);

      CHECK_THROWS(euler(test, -3, gamma, no_vax, vel_vax, eff_vax));
      CHECK_THROWS(euler(test, beta, -3, no_vax, vel_vax, eff_vax));
      CHECK_THROWS(euler(test, beta, gamma, -3, vel_vax, eff_vax));
      CHECK_THROWS(euler(test, beta, gamma, no_vax, 4, eff_vax));
      CHECK_THROWS(euler(test, beta, gamma, no_vax, vel_vax, 7));

      CHECK_THROWS(rk4(test, -3, gamma, no_vax, vel_vax, eff_vax));
      CHECK_THROWS(rk4(test, beta, -3, no_vax, vel_vax, eff_vax));
      CHECK_THROWS(rk4(test, beta, gamma, -3, vel_vax, eff_vax));
      CHECK_THROWS(rk4(test, beta, gamma, 4459000, vel_vax, eff_vax));
      CHECK_THROWS(rk4(test, beta, gamma, no_vax, 4, eff_vax));
      CHECK_THROWS(rk4(test, beta, gamma, no_vax, vel_vax, 7));
   } 
 
    SUBCASE("testing vector state") {
       int const people = 44590;
       int const M_t0 = 110;
       int const V_t0 = 1200;
       int const R_t0 = 0;
       int const S_t0 = people - M_t0 - V_t0 - R_t0;
       State init{S_t0, M_t0, R_t0, V_t0};
       Infection prova{100, init};

       CHECK(prova.get_M() == M_t0);
       CHECK(prova.get_S() == S_t0);
       CHECK(prova.get_R() == R_t0);
       CHECK(prova.get_V() == V_t0);
    }

    SUBCASE("testing euler"){
       int const people = 4459000;  
       int const M_t0 = 12321;
       int const V_t0 = 600437;
       int const R_t0 = 320000;
       int const S_t0 = people - M_t0 - V_t0 - R_t0;
       int duration = 100;
       double beta = 0.5; 
       double gamma = 0.3;
       int no_vax = 118292;   
       double vel_vax = 0.035;  
       double eff_vax = 0.600;
       State init{S_t0, M_t0, R_t0, V_t0};

       Infection prova{duration, init};
       prova = euler(prova, beta, gamma, no_vax, vel_vax, eff_vax);

       CHECK(prova.get_state(0).M < prova.get_state(1).M);
       CHECK(prova.get_state(0).V < prova.get_state(1).V);
       CHECK(prova.get_state(0).R < prova.get_state(1).R);
       CHECK(prova.get_state(0).S > prova.get_state(1).S);
       CHECK(prova.get_state(1).M < prova.get_state(2).M);
       CHECK(prova.get_state(1).V < prova.get_state(2).V);
       CHECK(prova.get_state(1).R < prova.get_state(2).R);
       CHECK(prova.get_state(1).S > prova.get_state(2).S);
    }
  
    SUBCASE("testing RK4") {
       int const people = 4459000;  
       int const M_t0 = 12321;
       int const V_t0 = 600437;
       int const R_t0 = 320000;
       int const S_t0 = people - M_t0 - V_t0 - R_t0;
       int duration = 100;
       double beta = 0.5; 
       double gamma = 0.3;
       int no_vax = 118292;   
       double vel_vax = 0.035;  
       double eff_vax = 0.600;
       State init{S_t0, M_t0, R_t0, V_t0};

       Infection prova{duration, init};
       prova = rk4(prova, beta, gamma, no_vax, vel_vax, eff_vax);

       CHECK(prova.get_state(0).M < prova.get_state(1).M);
       CHECK(prova.get_state(0).V < prova.get_state(1).V);
       CHECK(prova.get_state(0).R < prova.get_state(1).R);
       CHECK(prova.get_state(0).S > prova.get_state(1).S);
       CHECK(prova.get_state(1).M < prova.get_state(2).M);
       CHECK(prova.get_state(1).V < prova.get_state(2).V);
       CHECK(prova.get_state(1).R < prova.get_state(2).R);
       CHECK(prova.get_state(1).S > prova.get_state(2).S);
    }
    
    
}