#include "infection.hpp"
#include "display.hpp"

#include <iostream>
#include <stdexcept>

int main()
{
  int const people = 4459000; // abitanti emilia romagna
  int const M_t0 = 12321;
  int const V_t0 = 600447;
  int const R_t0 = 320000;
  int const S_t0 = people - M_t0 - V_t0 - R_t0;
  int duration;
  double beta, gamma;
  std::cout << "Duration of the simulation in days: " << '\t';
  std::cin >> duration;
  std::cout << "Beta (infectiousness) in ]0,1[: " << '\t';
  std::cin >> beta;
  if (beta >= 1 || beta <= 0)
    throw std::runtime_error{"unacceptable value, beta in ]0,1[."};
  std::cout << "Gamma (inverse of healing time in days) in ]0,1[: " << '\t';
  std::cin >> gamma;
  if (gamma >= 1 || gamma <= 0)
    throw std::runtime_error{"unacceptable value, gamma in ]0,1[."};
  epidemic::State init{S_t0, M_t0, R_t0, V_t0};
  epidemic::Infection Emilia_Romagna1{duration, init, people};
  int method;
  std::cout << "Numerical method (Euler -> press'0' / RK4 -> press '1'):" << '\t';
  std::cin >> method;
  if (method == 0)
  {
    Emilia_Romagna1.evolve(beta, gamma);
  }
  else if (method == 1)
  {
    Emilia_Romagna1.RK4(beta, gamma);
  }
  else
  {
    throw std::runtime_error{"Enter either '0' or '1'"};
  }
  epidemic::print(Emilia_Romagna1);
}