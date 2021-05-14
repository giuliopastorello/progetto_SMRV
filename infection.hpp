#ifndef INFECTION_HPP
#define INFECTION_HPP

#include <math.h>
#include <iostream>
#include <cassert>
#include <vector>


struct State {
  int S;  // suscettibili
  int M;  // rimossi
  int R;  // malati
  int V;  // vaccinati
};

class Infection {
  int m_duration_analysis_indays;  // durata analisi dati
  State init_state; //stato iniziale giorno:0
  int const m_N; // abitanti

 public:
  Infection(int duration_analysis_indays, State initial_state, int N)
      : m_duration_analysis_indays{duration_analysis_indays},
        init_state{initial_state},
        m_N{N} {}

  std::vector<State> SMRV_evolve() const {
    // double beta, double gamma, int non_vaccinabl, int M_t0, int V_t0, double
    // vel_vaccini, double eff_vaccino, int R_t0
    double const beta = 0.056;
    double const gamma = 0.045;
    int const non_vaccinabl = 118292;  // eta
    double const vel_vaccini = 0.05;   // mu
    double const eff_vaccino = 0.839;  // efficacia vaccino pesata sui vaccini distribuiti

    std::vector<State> result{init_state};

    State support = result.back();

    for (int i = 1; i < m_duration_analysis_indays; ++i) {
      assert(result.back().S + result.back().M + result.back().R + result.back().V == m_N);  // assert con vincolo

      support.V = round(result.back().V + vel_vaccini * (result.back().V / eff_vaccino) * (1 - result.back().V / (eff_vaccino * (m_N - non_vaccinabl))));
      support.M = round(result.back().M + beta * (result.back().S / m_N) * result.back().M - gamma * result.back().M);
      support.S = round(result.back().S - beta * (result.back().S / m_N) * result.back().M - vel_vaccini * (result.back().V / eff_vaccino) *
                  (1 - result.back().V / (eff_vaccino * (m_N - non_vaccinabl))));
      support.R = round(result.back().R + gamma * result.back().M);
      result.push_back(support);
    }

    return result;
  }

  void tabulate(std::ostream& os, std::vector<State> inf) {
    int const N= 52;
    int const L= inf.size();
    
    os << '+' << std::string(N, '-') << "+\n";
    os << "| DAYS | MALATI | RIMOSSI | VACCINATI | SUSCETTIBILI |\n";
    for (int r = 0; r < L; ++r) {
      os << "| " << r+1 << " | " << inf[r].M << " | " << inf[r].R << " | " << inf[r].V << " | " << inf[r].S << " |\n";
    }
    os << '+' << std::string(N, '-') << "+\n";
  }



};


#endif