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

  void Infection::evolve(double beta, double gamma, int no_vax, double vel_vax, double eff_vax) {
    if (beta >= 1 || beta <= 0) {
      throw std::runtime_error{"unacceptable value, beta in ]0,1[."};
    }
    if (gamma >= 1 || gamma <= 0) {
      throw std::runtime_error{"unacceptable value, gamma in ]0,1[."};
    }
    if (no_vax < 0) {
      throw std::runtime_error{"no vax people can't be negative."};
    }
    if (no_vax > m_N){
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

    for (int i = 1; i < m_time_indays; ++i) {
      
      double delV = vel_vax * (v() / eff_vax) * (1 - v() / (eff_vax * (m_N - no_vax)));

      double delS = (-beta * (s() / m_N) * m()) - delV;

      double delR = gamma * m();

      support.V = round(v() + h * delV);
      support.S = round(s() + h * delS);
      support.R = round(r() + h * delR);
      support.M = round(m() - h * (delV + delS + delR));

      //while (!(support.S + support.M + support.R + support.V == m_N)){
          if (support.S + support.M + support.R + support.V < m_N) {
              ++support.S;
          }
          if (support.S + support.M + support.R + support.V > m_N) {
              --support.S;
          }
      //}

      assert(support.M + support.R + support.S + support.V == m_N);

      assert(no_vax < m_N);

      m_data.push_back(support);
    }
  }

  void Infection::RK4(double beta, double gamma, int no_vax, double vel_vax, double eff_vax) {
    if (beta >= 1 || beta <= 0) {
      throw std::runtime_error{"unacceptable value, beta in ]0,1[."};
    }
    if (gamma >= 1 || gamma <= 0) {
      throw std::runtime_error{"unacceptable value, gamma in ]0,1[."};
    }
    if (no_vax < 0) {
      throw std::runtime_error{"no vax people can't be negative."};
    }
    if (no_vax > m_N){
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

    for (int i = 1; i < m_time_indays; ++i) {

      double a1 = vel_vax * (v() / eff_vax) * (1 - v() / (eff_vax * (m_N - no_vax)));
      double b1 = -beta * s() * m() / m_N - a1;
      double c1 = gamma * m();
      double d1 = -a1 - b1 - c1;

      double a2 = vel_vax * ((v() + a1 * h / 2) / eff_vax) * (1 - (v() + a1 * h / 2) / (eff_vax * (m_N - no_vax)));
      double b2 = -beta * (s() + b1 * h / 2) * (m() + d1 * h / 2) / m_N - a2;
      double c2 = gamma * (m() + d1 * h / 2);
      double d2 = -a2 - b2 - c2;

      double a3 = vel_vax * ((v() + a2 * h / 2) / eff_vax) * (1 - (v() + a2 * h / 2) / (eff_vax * (m_N - no_vax)));
      double b3 = -beta * (s() + b2 * h / 2) * (m() + d2 * h / 2) / m_N - a3;
      double c3 = gamma * (m() + d2 * h / 2);
      double d3 = -a3 - b3 - c3;

      double a4 = vel_vax * ((v() + a3 * h) / eff_vax) * (1 - (v() + a3 * h) / (eff_vax * (m_N - no_vax)));
      double b4 = -beta * (s() + b3 * h) * (m() + d3 * h) / m_N - a4;
      double c4 = gamma * (m() + d3 * h);
      double d4 = -a4 - b4 - c4;

      support.V = round(v() + (h / 6) * (a1  + 2 * a2  + 2 * a3 + a4));
      support.S = round(s() + (h / 6) * (b1  + 2 * b2  + 2 * b3 + b4));
      support.R = round(r() + (h / 6) * (c1  + 2 * c2  + 2 * c3 + c4));
      support.M = round(m() + (h / 6) * (d1  + 2 * d2  + 2 * d3 + d4));

     // while (!(support.S + support.M + support.R + support.V == m_N)){
          if (support.S + support.M + support.R + support.V < m_N) {
              ++support.S;
          }
          if (support.S + support.M + support.R + support.V > m_N) {
              --support.S;
          }
     //}

      assert(support.M + support.R + support.S + support.V == m_N);

      assert(no_vax < m_N);

      m_data.push_back(support);
    }
  }
}