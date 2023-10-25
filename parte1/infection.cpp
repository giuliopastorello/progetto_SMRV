#include "infection.hpp"

#include <cmath>

namespace epidemic
{
  std::vector<State> Infection::states() const { return analysis; }

  int Infection::s() { return analysis.back().S; }

  int Infection::m() { return analysis.back().M; }

  int Infection::r() { return analysis.back().R; }

  int Infection::v() { return analysis.back().V; }

  State Infection::get_state(int day) const { return analysis[day]; };

  void Infection::evolve(double beta, double gamma)
  {
    double const h = 1;
    int const non_vax = 118292;   // eta
    double const vel_vax = 0.05;  // mu
    double const eff_vax = 0.839; // efficacia vaccino
    for (int i = 1; i < m_duration_analysis_indays; ++i)
    {
      double delV = vel_vax * (v() / eff_vax) *
                    (1 - v() / (eff_vax * (m_N - non_vax)));
      double delS = -beta * (s() / m_N) * m() -
                    vel_vax * (v() / eff_vax) *
                        (1 - v() / (eff_vax * (m_N - non_vax)));
      double delR = gamma * m();

      support.V = round(v() + h * delV);
      support.S = round(s() + h * delS);
      support.R = round(r() + h * delR);
      support.M = round(m() - h * (delV + delS + delR));

      analysis.push_back(support);
    }
  }

  int count_digit(int n)
  {
    return std::log10(n) + 1;
  }

  void Infection::print() const
  {
    int const N = analysis[0].S + analysis[0].M + analysis[0].R + analysis[0].V;
    int const width = std::log10(N) + 4;

    std::cout << '|' << " day   " << std::string(floor(width / 2) - 1, ' ') << 'S' << std::string(floor(width / 2) - 1, ' ')
              << std::string(floor(width / 2) - 1, ' ') << 'M' << std::string(floor(width / 2) - 1, ' ')
              << std::string(floor(width / 2) - 1, ' ') << 'R' << std::string(floor(width / 2) - 1, ' ')
              << std::string(floor(width / 2) - 1, ' ') << 'V' << std::string(floor(width) - 2, ' ')
              << '|' << '\n';

    for (int i = 0; i < m_duration_analysis_indays; i++)
    {
      std::cout << '|' << std::string(2, ' ') << i + 1 << ')' << std::string(4 - std::log10(i + 1.5), ' ')
                << analysis[i].S
                << std::string(width - count_digit(analysis[i].S), ' ') << analysis[i].M
                << std::string(width - count_digit(analysis[i].M), ' ') << analysis[i].R
                << std::string(width - count_digit(analysis[i].R), ' ') << analysis[i].V
                << std::string(width - count_digit(analysis[i].V), ' ') << '|' << '\n';
      //<< '+' << std::string(width - 1, '-') << '+'
      //<< std::string(width - 1, '-') << '+' << std::string(width - 1, '-')
      //<< '+' << std::string(width, '-') << '+' << '\n';
    }
  }
}