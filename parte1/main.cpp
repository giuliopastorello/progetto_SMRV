#include "infection.hpp"
#include "display.hpp"

int main() {

  int const people = 4459000; // abitanti emilia romagna
  int M_t0 = 23321;
  int V_t0 = 400007;
  int R_t0 = 320000;
  int S_t0 = people - M_t0 - V_t0 - R_t0;
  int duration = 50;
  double beta = 0.5; 
  double gamma = 0.3;
  int no_vax = 118292;   // eta
  double vel_vax = 0.04;  // mu
  double eff_vax = 0.700; // xi

  //ingresso parametri
  std::string answer = "no";
  std::cout << "Do you want to change standard parameters?: (yes/no)" << '\t';
  std::cin >> answer;
  if (answer == "yes") {
    std::cout << "Duration of the simulation in days: " << '\t';
    std::cin >> duration;
    std::cout << "Number of susceptibles: " << '\t';
    std::cin >> S_t0;
    std::cout << "Number of removed: " << '\t';
    std::cin >> R_t0;
    std::cout << "Number of vaccinated: " << '\t';
    std::cin >> V_t0;
    std::cout << "Number of infected: " << '\t';
    std::cin >> M_t0;
    std::cout << "Beta (infectiousness) in ]0,1[: " << '\t';
    std::cin >> beta;
    std::cout << "Gamma (inverse of healing time in days) in ]0,1[: " << '\t';
    std::cin >> gamma;
    std::cout << "Mu (vaccine velocity) in ]0,1[: " << '\t';
    std::cin >> vel_vax;
    std::cout << "Xi (vaccine efficiency) in ]0,1[: " << '\t';
    std::cin >> eff_vax;
    std::cout << "Number of no vax people (< S): " << '\t';
    std::cin >> no_vax;
    if (no_vax > S_t0 + R_t0 + V_t0 + M_t0){
       throw std::runtime_error{"no vax must be less than total people"};
    }
  }
    
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
  
}