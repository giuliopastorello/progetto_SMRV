#include "infection.hpp"

#include <cmath>

namespace epidemic
{
  std::vector<State> Infection::states() const { return analysis; }

  double Infection::s() { return analysis.back().S; }

  double Infection::m() { return analysis.back().M; }

  double Infection::r() { return analysis.back().R; }

  double Infection::v() { return analysis.back().V; }

  void Infection::evolve(double beta, double gamma)
  {
    // double const beta = 0.056;
    // double const gamma = 0.045;
    int const non_vax = 118292;   // eta
    double const vel_vax = 0.05;  // mu
    double const eff_vax = 0.839; // efficacia vaccino
    for (int i = 1; i < m_duration_analysis_indays; ++i)
    {
      // if (s() + m() + r() + v() < m_N)
      //{
      //   ++analysis.back().S;
      // }
      // if (s() + m() + r() + v() > m_N)
      //{
      //   --analysis.back().S;
      // }

      support.S = round(s() - beta * (s() / m_N) * m() -
                        vel_vax * (v() / eff_vax) *
                            (1 - v() / (eff_vax * (m_N - non_vax))));
      support.M = round(m() + beta * (s() / m_N) * m() - gamma * m());
      support.R = round(r() + gamma * m());

      support.V = round(v() + vel_vax * (v() / eff_vax) *
                                  (1 - v() / (eff_vax * (m_N - non_vax))));

      analysis.push_back(support);

      // if (support.S < 0 || support.M < 0 || support.R < 0 || support.V < 0) {
      //   std::cout << "Errore" << '\n';
      //   break;
      // }
    }
  }

  int count_digit(int n)
  {
    return std::log10(n) + 1;
  }

  void Infection::print(State state) const
  {
    int const N = state.S + state.M + state.R + state.V;
    int const width = std::log10(N) + 4;

    std::cout << '|' << state.S
              << std::string(width - count_digit(state.S), ' ') << state.M
              << std::string(width - count_digit(state.M), ' ') << state.R
              << std::string(width - count_digit(state.R), ' ') << state.V
              << std::string(width - count_digit(state.V), ' ') << '|' << '\n';
              //<< '+' << std::string(width - 1, '-') << '+'
              //<< std::string(width - 1, '-') << '+' << std::string(width - 1, '-')
              //<< '+' << std::string(width, '-') << '+' << '\n';
  }

// void Infection::tabulate(
//   std::ostream& os) {  // ancora problemi di formattazione della tabella:
//                        // trovare dato più lungo : m_N abitanti
// int const N = 52;      // meglio mettere la funzione dentro e utilizzare come
//                        // membro un std::vector<State>
// int const L = analysis.size();
// std::string space = std::to_string(m_N);
//
// os << '+' << std::string(N, '-') << "+\n";
// os << "| DAYS | MALATI | RIMOSSI | VACCINATI | SUSCETTIBILI |\n";
// for (int r = 0; r < L; ++r) {
//  os << "     " << r + 1 << "        " << analysis[r].M << "        "
//     << analysis[r].R << "        " << analysis[r].V << "        "
//     << analysis[r].S << "    \n";
//
//}  // r+1 così inizia da 1 e finisce a L il processo
// os << '+' << std::string(N, '-') << "+\n";
}
