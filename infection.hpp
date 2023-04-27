#ifndef INFECTION_HPP
#define INFECTION_HPP

#include <math.h>

#include <iostream>
#include <string>
#include <vector>

struct State {
  int S;  // suscettibili
  int M;  // rimossi
  int R;  // malati
  int V;  // vaccinati
};

class Infection {
  int m_duration_analysis_indays;  // durata analisi dati
  std::vector<State> analysis;     // stato iniziale giorno:0
  int const m_N;                   // abitanti

 public:
  Infection(int duration_analysis_indays, State initial_state, int N)
      : m_duration_analysis_indays{duration_analysis_indays},
        analysis{initial_state},
        m_N{N} {}
  
  double s();
  double m();
  double r();
  double v();
  void evolve();

  State support = analysis.back();
  State get_state(int day) const { return analysis[day]; };

  void tabulate(std::ostream& os);
};

#endif
