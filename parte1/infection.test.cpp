#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "infection.hpp"

TEST_CASE("testing virus") {
  int const people = 4459000;  // abitanti emilia romagna
  int const M_t0 = 12321;
  int const V_t0 = 600437;
  int const R_t0 = 320000;
  int const S_t0 = people - M_t0 - V_t0 - R_t0;
  epidemic::State init{S_t0, M_t0, R_t0, V_t0};
  epidemic::Infection prova(100, init);
}

TEST_CASE("testing virus") {
  int const people = 44590;
  int const M_t0 = 110;
  int const V_t0 = 1200;
  int const R_t0 = 0;
  int const S_t0 = people - M_t0 - V_t0 - R_t0;
  epidemic::State init{S_t0, M_t0, R_t0, V_t0};
  epidemic::Infection prova{100, init};
}

TEST_CASE("testing virus") {
  int const people = 0;
  int const M_t0 = 0;
  int const V_t0 = 0;
  int const R_t0 = 0;
  int const S_t0 = people - M_t0 - V_t0 - R_t0;
  epidemic::State init{S_t0, M_t0, R_t0, V_t0};
  epidemic::Infection prova{100, init};
}

TEST_CASE("testing virus") {
  int const people = 44590;
  int const M_t0 = -110;
  int const V_t0 = 1200;
  int const R_t0 = 0;
  int const S_t0 = people - M_t0 - V_t0 - R_t0;
  epidemic::State init{S_t0, M_t0, R_t0, V_t0};
  epidemic::Infection prova{100, init};
}