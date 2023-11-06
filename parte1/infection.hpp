#ifndef INFECTION_HPP
#define INFECTION_HPP

#include <math.h>
#include <iostream>
#include <string>
#include <vector>

namespace epidemic {
 
  struct State {
    int S; // suscettibili
    int M; // rimossi
    int R; // malati
    int V; // vaccinati
  };

  class Infection {

    int m_time_indays; // durata analisi dati
    std::vector<State> analysis; // stato iniziale giorno:0
    int const m_N; // abitanti

    public:

       Infection(int days, State const &initial_state, int N)
           : m_time_indays{days},
             analysis{initial_state},
             m_N{N} {}
             
       int s() const;
       int m() const;
       int r() const;
       int v() const;
   
       std::vector<int> get_S_vector() const;
       std::vector<int> get_M_vector() const;
       std::vector<int> get_R_vector() const;
       std::vector<int> get_V_vector() const;
   
       std::vector<State> states() const;

       void evolve(double beta, double gamma);
       void RK4(double beta, double gamma);
   
       State support = analysis.back();
       State get_state(int day) const;
   
       void print() const;
   
       void graph();
  };

  int count_digit(int n);

}//namespace epidemic

#endif
