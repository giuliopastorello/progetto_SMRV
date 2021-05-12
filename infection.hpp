#ifndef INFECTION_PROJECT_HPP
#define INFECTION_PROJECT_HPP

#include <vector>
#include <cassert>
#include <math.h>

struct State{
   int S; //suscettibili
   int M; //rimossi
   int R; //malati
   int V; //vaccinati
};


class Infection {
   int m_duration_analysis_indays;  //durata analisi dati
   State m_initial_state;
   int const m_N;

   public:

   Infection(int duration_analysis_indays, State initial_state, int N):
   m_duration_analysis_indays{duration_analysis_indays}, m_initial_state{initial_state}, m_N{N} {}

   std::vector<State> SMRV() const {
       //double beta, double gamma, int non_vaccinabl, int M_t0, int V_t0, double vel_vaccini, double eff_vaccino, int R_t0
       double const beta = 0.056;
       double const gamma= 0.045;
       int const  non_vaccinabl= 118292; //eta
       double const vel_vaccini=0.05; //mu
       double const eff_vaccino=0.839; //efficacia vaccino pesata sui vaccini distribuiti 
       
       std::vector<State> result{m_initial_state};

       State support=result.back();

       for (int i=1; i<m_duration_analysis_indays; ++i){

           assert(result.back().S + result.back().M + result.back().R + result.back().V == m_N); //assert con vincolo

           support.V=round(result.back().V + vel_vaccini*(result.back().V / eff_vaccino)*(1- result.back().V / (eff_vaccino*(m_N-non_vaccinabl))));
           support.M=round(result.back().M + beta*(result.back().S / m_N)*result.back().M - gamma*result.back().M);
           support.S=round(result.back().S -beta*(result.back().S/m_N)*result.back().M - vel_vaccini*(result.back().V / eff_vaccino)*(1- result.back().V / (eff_vaccino*(m_N-non_vaccinabl))));
           support.R=round(result.back().R + gamma*result.back().M);
           result.push_back(support);
       }
       
    }

    void print() const{
        

    }

}; 

#endif