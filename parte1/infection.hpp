#ifndef INFECTION_HPP
#define INFECTION_HPP

#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>
#include <matplot/matplot.h>

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
    int const m_N; // abitanti

    public:

       Infection(int days, State const &initial_state, int N)
           : m_time_indays{days},
             m_data{initial_state},
             m_N{N} {}

       //get      
       int s() const;
       int m() const;
       int r() const;
       int v() const;
   
       std::vector<int> get_S_vector() const;
       std::vector<int> get_M_vector() const;
       std::vector<int> get_R_vector() const;
       std::vector<int> get_V_vector() const;

       State get_state(int day) const;
   
       //std::vector<State> states() const; non necessario

       void evolve(double beta, double gamma, int no_vax, double vel_vax, double eff_vax);
       void RK4(double beta, double gamma, int no_vax, double vel_vax, double eff_vax);
       
       void print() const;
   
       void graph();
  };

  int count_digit(int n);

}//namespace epidemic

#endif
