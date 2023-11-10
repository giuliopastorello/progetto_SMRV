#include "infection.hpp"

namespace epidemic {

  //std::vector<State> Infection::get_states() const { return m_data; }

  Infection::Infection(int days, State const &initial_state)
         : m_time_indays{days},
           m_data{initial_state},
           m_N{initial_state.M + initial_state.R + initial_state.S + initial_state.V}  {
              if(days < 10) {
                throw std::runtime_error{"Virus must last at least 10 days"};
              }
              if(initial_state.M < 1) {
                throw std::runtime_error{"There must be at least one infected"};
              }
              if(initial_state.S < 1) {
                throw std::runtime_error{"There must be at least one susceptible"};
              }
           }

  Infection::Infection(Infection const &infection){
    m_time_indays = infection.m_time_indays;
    m_N = infection.m_N;
    m_data = infection.m_data;
  }

  int Infection::get_S() const { return m_data.back().S; }

  int Infection::get_M() const { return m_data.back().M; }

  int Infection::get_R() const { return m_data.back().R; }

  int Infection::get_V() const { return m_data.back().V; }

  int Infection::get_N() const { return m_N; }

  void Infection::set_laststate(State const &state) {
      m_data.push_back(state);
  }

  int Infection::get_days() const {return m_time_indays; }

  State Infection::get_state(int day) const { return m_data[day]; };

  State Infection::get_laststate() const { return m_data.back();}

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

  Infection euler(Infection const &plague, double beta, double gamma, int no_vax, double vel_vax, double eff_vax) {

    if (beta >= 1 || beta <= 0) {
      throw std::runtime_error{"unacceptable value, beta in ]0,1[."};
    }
    if (gamma >= 1 || gamma <= 0) {
      throw std::runtime_error{"unacceptable value, gamma in ]0,1[."};
    }
    if (no_vax < 0) {
      throw std::runtime_error{"no vax people can't be negative."};
    }
    if (no_vax >= plague.get_N()){
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

    Infection result(plague);

    for (int i = 1; i < result.get_days(); ++i) {
      
      double delV = vel_vax * result.get_V() / eff_vax * 
                    (1 - result.get_V() / (eff_vax * (result.get_N() - no_vax)));

      double delS = (-beta * (result.get_S() / result.get_N()) * result.get_M() - vel_vax * result.get_V() / eff_vax * 
                    (1 - result.get_V() / (eff_vax * (result.get_N() - no_vax))));

      double delR = gamma * result.get_M();

      double delM = beta * result.get_S() / result.get_N() * result.get_M() - gamma * result.get_M();

      support.V = round(result.get_V() + h * delV);
      support.S = round(result.get_S() + h * delS);
      support.R = round(result.get_R() + h * delR);
      support.M = round(result.get_M() + h * delM);

      while (!(support.S + support.M + support.R + support.V == result.get_N())){
      if (support.S + support.M + support.R + support.V < result.get_N()) {
          ++support.S;
      }
      if (support.S + support.M + support.R + support.V > result.get_N()) {
          --support.S;
      }
      }

      assert(support.M + support.R + support.S + support.V == result.get_N());

      assert(no_vax < result.get_N());

      result.set_laststate(support);
    }

    return result;

  }

  Infection rk4(Infection const &plague, double beta, double gamma, int no_vax, double vel_vax, double eff_vax) {

    if (beta >= 1 || beta <= 0) {
      throw std::runtime_error{"unacceptable value, beta in ]0,1[."};
    }
    if (gamma >= 1 || gamma <= 0) {
      throw std::runtime_error{"unacceptable value, gamma in ]0,1[."};
    }
    if (no_vax < 0) {
      throw std::runtime_error{"no vax people can't be negative."};
    }
    if (no_vax >= plague.get_N()){
       throw std::runtime_error{"no vax must be less than total people"};
    }
    if (vel_vax >= 1 || vel_vax <= 0) {
      throw std::runtime_error{"unacceptable value, vel_vax in ]0,1[."};
    }
    if (eff_vax >= 1 || eff_vax<= 0) {
      throw std::runtime_error{"unacceptable value, eff_vax in ]0,1[."};
    }

    double const h = 1;

    State support;

    Infection result(plague);

    for (int i = 1; i < result.get_days(); ++i) {

      double a1 = vel_vax * (result.get_V() / eff_vax) * (1 - result.get_V() / (eff_vax * (result.get_N() - no_vax)));
      double b1 = -beta * result.get_S() * result.get_M() / result.get_N() - a1;
      double c1 = gamma * result.get_M();
      double d1 = -a1 - b1 - c1;

      double a2 = vel_vax * ((result.get_V() + a1 * h / 2) / eff_vax) * (1 - (result.get_V() + a1 * h / 2) / (eff_vax * (result.get_N() - no_vax)));
      double b2 = -beta * (result.get_S() + b1 * h / 2) * (result.get_M() + d1 * h / 2) / result.get_N() - a2;
      double c2 = gamma * (result.get_M() + d1 * h / 2);
      double d2 = -a2 - b2 - c2;

      double a3 = vel_vax * ((result.get_V() + a2 * h / 2) / eff_vax) * (1 - (result.get_V() + a2 * h / 2) / (eff_vax * (result.get_N() - no_vax)));
      double b3 = -beta * (result.get_S() + b2 * h / 2) * (result.get_M() + d2 * h / 2) / result.get_N() - a3;
      double c3 = gamma * (result.get_M() + d2 * h / 2);
      double d3 = -a3 - b3 - c3;

      double a4 = vel_vax * ((result.get_V() + a3 * h) / eff_vax) * (1 - (result.get_V() + a3 * h) / (eff_vax * (result.get_N() - no_vax)));
      double b4 = -beta * (result.get_S() + b3 * h) * (result.get_M() + d3 * h) / result.get_N() - a4;
      double c4 = gamma * (result.get_M() + d3 * h);
      double d4 = -a4 - b4 - c4;

      support.V = round(result.get_V() + (h / 6) * (a1  + 2 * a2  + 2 * a3 + a4));
      support.S = round(result.get_S() + (h / 6) * (b1  + 2 * b2  + 2 * b3 + b4));
      support.R = round(result.get_R() + (h / 6) * (c1  + 2 * c2  + 2 * c3 + c4));
      support.M = round(result.get_M() + (h / 6) * (d1  + 2 * d2  + 2 * d3 + d4));

      while (!(support.S + support.M + support.R + support.V == result.get_N())){
          if (support.S + support.M + support.R + support.V < result.get_N()) {
              ++support.S;
          }
          if (support.S + support.M + support.R + support.V > result.get_N()) {
              --support.S;
          }
      }

      assert(support.M + support.R + support.S + support.V == result.get_N());

      assert(no_vax < result.get_N());

      result.set_laststate(support);
    }

    return result;
  }
}