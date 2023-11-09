#include "infection.hpp"

namespace epidemic
{
  //std::vector<State> Infection::states() const { return m_data; }

  Infection::Infection(int days, State const &initial_state)
         : m_time_indays{days},
           m_data{initial_state},
           m_N{initial_state.M + initial_state.R + initial_state.S + initial_state.V}  {
              if(initial_state.M < 1) {
                throw std::runtime_error{"There must be at least one infected"};
              }
              if(initial_state.S < 1) {
                throw std::runtime_error{"There must be at least one susceptible"};
              }
           }

  int Infection::s() const { return m_data.back().S; }

  int Infection::m() const { return m_data.back().M; }

  int Infection::r() const { return m_data.back().R; }

  int Infection::v() const { return m_data.back().V; }

  int Infection::get_N() const { return m_N; }

  void Infection::set_laststate(State const &state) {
      m_data.push_back(state);
  }

  int Infection::get_days() const {return m_time_indays; }

  State Infection::get_state(int day) const { return m_data[day]; };

  std::vector<int> Infection::get_S_vector() const {
    int const N = m_data.size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i) {
      int num_s = m_data[i].S;
      result.push_back(num_s);
    }
    assert(m_data.size() == result.size());
    return result;
  }

  std::vector<int> Infection::get_R_vector() const {
    int const N = m_data.size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i) {
      int num_r = m_data[i].R;
      result.push_back(num_r);
    }
    assert(m_data.size() == result.size());
    return result;
  }

  std::vector<int> Infection::get_V_vector() const {
    int const N = m_data.size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i) {
      int num_v = m_data[i].V;
      result.push_back(num_v);
    }
    assert(m_data.size() == result.size());
    return result;
  }

  std::vector<int> Infection::get_M_vector() const {
    int const N = m_data.size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i) {
      int num_m = m_data[i].M;
      result.push_back(num_m);
    }
    assert(m_data.size() == result.size());
    return result;
  }

  void evolve(Infection &plague, double beta, double gamma, int no_vax, double vel_vax, double eff_vax) {
    if (beta >= 1 || beta <= 0) {
      throw std::runtime_error{"unacceptable value, beta in ]0,1[."};
    }
    if (gamma >= 1 || gamma <= 0) {
      throw std::runtime_error{"unacceptable value, gamma in ]0,1[."};
    }
    if (no_vax < 0) {
      throw std::runtime_error{"no vax people can't be negative."};
    }
    if (no_vax > plague.get_N()){
       throw std::runtime_error{"no vax must be less than total people"};
    }
    if (vel_vax >= 1 || vel_vax <= 0) {
      throw std::runtime_error{"unacceptable value, vel_vax in ]0,1[."};
    }
    if (eff_vax >= 1 || eff_vax <= 0) {
      throw std::runtime_error{"unacceptable value, eff_vax in ]0,1[."};
    }
    
    double const h = 1;

    State support;

    for (int i = 1; i < plague.get_days(); ++i) {
      
      double delV = vel_vax * (plague.v() / eff_vax) * 
                    (1 - plague.v() / (eff_vax * (plague.get_N() - no_vax)));

      double delS = (-beta * (plague.s() / plague.get_N()) * plague.m()) - delV;

      double delR = gamma * plague.m();

      support.V = round(plague.v() + h * delV);
      support.S = round(plague.s() + h * delS);
      support.R = round(plague.r() + h * delR);
      support.M = round(plague.m() - h * (delV + delS + delR));

      //while (!(support.S + support.M + support.R + support.V == m_N)){
          if (support.S + support.M + support.R + support.V < plague.get_N()) {
              ++support.S;
          }
          if (support.S + support.M + support.R + support.V > plague.get_N()) {
              --support.S;
          }
      //}

      assert(support.M + support.R + support.S + support.V == plague.get_N());

      assert(no_vax < plague.get_N());

      plague.set_laststate(support);
    }

  }

  void RK4(Infection &plague, double beta, double gamma, int no_vax, double vel_vax, double eff_vax) {
    if (beta >= 1 || beta <= 0) {
      throw std::runtime_error{"unacceptable value, beta in ]0,1[."};
    }
    if (gamma >= 1 || gamma <= 0) {
      throw std::runtime_error{"unacceptable value, gamma in ]0,1[."};
    }
    if (no_vax < 0) {
      throw std::runtime_error{"no vax people can't be negative."};
    }
    if (no_vax > plague.get_N()){
       throw std::runtime_error{"no vax must be less than total people"};
    }
    if (vel_vax >= 1 || vel_vax <= 0) {
      throw std::runtime_error{"unacceptable value, vel_vax in ]0,1[."};
    }
    if (eff_vax >= 1 || eff_vax<= 0) {
      throw std::runtime_error{"unacceptable value, eff_vax in ]0,1[."};
    }

    double const h = 1; // step size

    State support;

    for (int i = 1; i < plague.get_days(); ++i) {

      double a1 = vel_vax * (plague.v() / eff_vax) * (1 - plague.v() / (eff_vax * (plague.get_N() - no_vax)));
      double b1 = -beta * plague.s() * plague.m() / plague.get_N() - a1;
      double c1 = gamma * plague.m();
      double d1 = -a1 - b1 - c1;

      double a2 = vel_vax * ((plague.v() + a1 * h / 2) / eff_vax) * (1 - (plague.v() + a1 * h / 2) / (eff_vax * (plague.get_N() - no_vax)));
      double b2 = -beta * (plague.s() + b1 * h / 2) * (plague.m() + d1 * h / 2) / plague.get_N() - a2;
      double c2 = gamma * (plague.m() + d1 * h / 2);
      double d2 = -a2 - b2 - c2;

      double a3 = vel_vax * ((plague.v() + a2 * h / 2) / eff_vax) * (1 - (plague.v() + a2 * h / 2) / (eff_vax * (plague.get_N() - no_vax)));
      double b3 = -beta * (plague.s() + b2 * h / 2) * (plague.m() + d2 * h / 2) / plague.get_N() - a3;
      double c3 = gamma * (plague.m() + d2 * h / 2);
      double d3 = -a3 - b3 - c3;

      double a4 = vel_vax * ((plague.v() + a3 * h) / eff_vax) * (1 - (plague.v() + a3 * h) / (eff_vax * (plague.get_N() - no_vax)));
      double b4 = -beta * (plague.s() + b3 * h) * (plague.m() + d3 * h) / plague.get_N() - a4;
      double c4 = gamma * (plague.m() + d3 * h);
      double d4 = -a4 - b4 - c4;

      support.V = round(plague.v() + (h / 6) * (a1  + 2 * a2  + 2 * a3 + a4));
      support.S = round(plague.s() + (h / 6) * (b1  + 2 * b2  + 2 * b3 + b4));
      support.R = round(plague.r() + (h / 6) * (c1  + 2 * c2  + 2 * c3 + c4));
      support.M = round(plague.m() + (h / 6) * (d1  + 2 * d2  + 2 * d3 + d4));

     // while (!(support.S + support.M + support.R + support.V == m_N)){
          if (support.S + support.M + support.R + support.V < plague.get_N()) {
              ++support.S;
          }
          if (support.S + support.M + support.R + support.V > plague.get_N()) {
              --support.S;
          }
     //}

      assert(support.M + support.R + support.S + support.V == plague.get_N());

      assert(no_vax < plague.get_N());

      plague.set_laststate(support);
    }
  }
}