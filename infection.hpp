#ifndef INFECTION_HPP
#define INFECTION_HPP

#include <math.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>


struct State {
  int S;  // suscettibili
  int M;  // rimossi
  int R;  // malati
  int V;  // vaccinati
};

class Infection {
  int m_duration_analysis_indays;  // durata analisi dati
  std::vector<State> analysis; //stato iniziale giorno:0
  int const m_N; // abitanti

 public:
  Infection(int duration_analysis_indays, State initial_state, int N)
      : m_duration_analysis_indays{duration_analysis_indays},
        analysis{initial_state},
        m_N{N} {}

  void SMRV_evolve() {
    // double beta, double gamma, int non_vaccinabl, int M_t0, int V_t0, double
    // vel_vaccini, double eff_vaccino, int R_t0
    double const beta = 0.056;
    double const gamma = 0.045;
    int const non_vaccinabl = 118292;  // etdaa
    double const vel_vaccini = 0.05;   // mu
    double const eff_vaccino = 0.839;  // efficacia vaccino pesata sui vaccini distribuiti

    State support = analysis.back();

    for (int i = 1; i < m_duration_analysis_indays; ++i) {
      assert(analysis.back().S + analysis.back().M + analysis.back().R + analysis.back().V == m_N);  // assert con vincolo

      support.V = round(analysis.back().V + vel_vaccini * (analysis.back().V / eff_vaccino) * (1 - analysis.back().V / (eff_vaccino * (m_N - non_vaccinabl))));
      support.M = round(analysis.back().M + beta * (analysis.back().S / m_N) * analysis.back().M - gamma * analysis.back().M);
      support.S = round(analysis.back().S - beta * (analysis.back().S / m_N) * analysis.back().M - vel_vaccini * (analysis.back().V / eff_vaccino) *
                  (1 - analysis.back().V / (eff_vaccino * (m_N - non_vaccinabl))));
      support.R = round(analysis.back().R + gamma * analysis.back().M);
      analysis.push_back(support);
    }

  }



  State get_state(int day) const{
    return analysis[day];
  }

  void tabulate(std::ostream& os) {  //ancora problemi di formattazione della tabella: trovare dato più lungo : m_N abitanti
    int const N= 52;                                              // meglio mettere la funzione dentro e utilizzare come membro un std::vector<State>
    int const L= analysis.size();
    std::string space = std::to_string(m_N);

    os << '+' << std::string(N, '-') << "+\n";
    os << "| DAYS | MALATI | RIMOSSI | VACCINATI | SUSCETTIBILI |\n";
    for (int r = 0; r < L; ++r) {
      os << "|     " << r+1 << "    |    " << analysis[r].M << "    |    " << analysis[r].R << "    |    " << analysis[r].V << "    |    " << analysis[r].S << "    |\n";
    } //r+1 così inizia da 1 e finisce a L il processo
    os << '+' << std::string(N, '-') << "+\n";
  }

  



};




#endif