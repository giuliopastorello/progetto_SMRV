#ifndef INFECTION_HPP
#define INFECTION_HPP

#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <stdexcept>

namespace epidemic {
 
  struct State {
    int S; // suscettibili
    int M; // rimossi
    int R; // malati
    int V; // vaccinati
  };

  class Infection {

    int m_time_indays; // durata analisi dati
    std::vector<State> m_data; // stato iniziale giorno:0
    int m_N; // abitanti

    public:

       Infection(int days, State const &initial_state);
       Infection(Infection const &infection);//costruttore copia
       Infection &operator=(Infection const &other) = default;
           
       //get      
       int get_S() const;
       int get_M() const;
       int get_R() const;
       int get_V() const;

       std::vector<int> get_S_vector() const;
       std::vector<int> get_M_vector() const;
       std::vector<int> get_R_vector() const;
       std::vector<int> get_V_vector() const;

       int get_N() const;
       int get_days() const;

       State get_state(int day) const;
       State get_laststate() const;

       //set
       void set_laststate(State const &state);
   
       //std::vector<State> get_states() const; non necessario
  };

  Infection evolve(Infection const &plague, double beta, double gamma, int no_vax, double vel_vax, double eff_vax);

  Infection RK4(Infection const &plague, double beta, double gamma, int no_vax, double vel_vax, double eff_vax);
  
}//namespace epidemic

#endif
