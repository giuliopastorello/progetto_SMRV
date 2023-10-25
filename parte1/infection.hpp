#ifndef INFECTION_HPP
#define INFECTION_HPP

#include <math.h>

#include <iostream>
#include <string>
#include <vector>

namespace epidemic
{
 
  struct State
  {
    int S; // suscettibili
    int M; // rimossi
    int R; // malati
    int V; // vaccinati
  };

  class Infection
  {
    int m_duration_analysis_indays; // durata analisi dati
    std::vector<State> analysis;    // stato iniziale giorno:0
    int const m_N;                  // abitanti

  public:
    Infection(int duration_analysis_indays, State initial_state, int N)
        : m_duration_analysis_indays{duration_analysis_indays},
          analysis{initial_state},
          m_N{N} {}
    
    //Get       
    int s();
    int m();
    int r();
    int v();

    std::vector<int> get_S();
    std::vector<int> get_M();
    std::vector<int> get_R();
    std::vector<int> get_V();

    std::vector<State> states () const;
    void evolve(double beta, double gamma);
    void RK4(double beta, double gamma);

    State support = analysis.back();
    State get_state(int day) const;

    void print() const;

    void graph();
  };
}
#endif