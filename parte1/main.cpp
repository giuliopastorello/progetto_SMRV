#include "infection.hpp"
#include "display.hpp"
#include <iostream>
#include <stdexcept>

int main() {

  int const people = 4459000; // abitanti emilia romagna
  int const M_t0 = 23321;
  int const V_t0 = 400007;
  int const R_t0 = 320000;
  int const S_t0 = people - M_t0 - V_t0 - R_t0;
  int duration = 50;
  double beta = 0.5; 
  double gamma = 0.3;
  int const no_vax = 118292;   // eta
  double const vel_vax = 0.04;  // mu
  double const eff_vax = 0.700; // xi
  
  std::cout << "Duration of the simulation in days: " << '\t';
  std::cin >> duration;
  std::cout << "Beta (infectiousness) in ]0,1[: " << '\t';
  std::cin >> beta;
 
  std::cout << "Gamma (inverse of healing time in days) in ]0,1[: " << '\t';
  std::cin >> gamma;
  
  epidemic::State init{S_t0, M_t0, R_t0, V_t0};
  epidemic::Infection Emilia_Romagna1{duration, init};

  int method;
  std::cout << "Numerical method (Euler -> press'0' / RK4 -> press '1'):" << '\t';
  std::cin >> method;

  if (method == 0) {
    Emilia_Romagna1.evolve(beta, gamma, no_vax, vel_vax, eff_vax);
    epidemic::print(Emilia_Romagna1);
    epidemic::graph(Emilia_Romagna1);

  } else if (method == 1) {
    Emilia_Romagna1.RK4(beta, gamma, no_vax, vel_vax, eff_vax);

    epidemic::print(Emilia_Romagna1);
    epidemic::graph(Emilia_Romagna1);
  } else {
    throw std::runtime_error{"Enter either '0' or '1'"};
  }

  return 0;
  
}