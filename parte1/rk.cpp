#include <math.h>

#include "infection.hpp"

namespace epidemic
{
  void Infection::RK4(double beta, double gamma)
  {
    float const h = 1; // step size
    // double const beta = 0.056;
    // double const gamma = 0.045;
    int const eta = 118292;  // no vax
    double const mu = 0.05;  // velocità vaccino
    double const xi = 0.839; // efficacia vaccino
    for (int i = 1; i < m_duration_analysis_indays; ++i)
    {
      //  if (s() + m() + r() + v() < m_N) {
      //    ++analysis.back().S;
      //  }
      //  if (s() + m() + r() + v() > m_N) {
      //    --analysis.back().S;
      //  }

      double a1 = mu * (v() / xi) * (1 - v() / (xi * (m_N - eta)));
      double b1 = -beta * s() * m() / m_N - a1;
      double c1 = gamma * m();
      double d1 = -a1 - b1 - c1;

      double a2 = mu * ((v() + a1 * h / 2) / xi) * (1 - (v() + a1 * h / 2) / (xi * (m_N - eta)));
      double b2 = -beta * (s() + b1 * h / 2) * (m() + d1 * h / 2) / m_N - a2;
      double c2 = gamma * (m() + d1 * h / 2);
      double d2 = -a2 - b2 - c2;

      double a3 = mu * ((v() + a2 * h / 2) / xi) * (1 - (v() + a2 * h / 2) / (xi * (m_N - eta)));
      double b3 = -beta * (s() + b2 * h / 2) * (m() + d2 * h / 2) / m_N - a3;
      double c3 = gamma * (m() + d2 * h / 2);
      double d3 = -a3 - b3 - c3;

      double a4 = mu * ((v() + a3 * h) / xi) * (1 - (v() + a3 * h) / (xi * (m_N - eta)));
      double b4 = -beta * (s() + b3 * h) * (m() + d3 * h) / m_N - a4;
      double c4 = gamma * (m() + d3 * h);
      double d4 = -a4 - b4 - c4;

      support.V = round(v() + (h / 6) * (a1  + 2 * a2  + 2 * a3 + a4));
      support.S = round(s() + (h / 6) * (b1  + 2 * b2  + 2 * b3 + b4));
      support.R = round(r() + (h / 6) * (c1  + 2 * c2  + 2 * c3 + c4));
      support.M = round(m() + (h / 6) * (d1  + 2 * d2  + 2 * d3 + d4));

      analysis.push_back(support);

      //if (support.S < 0 || support.M < 0 || support.R < 0 || support.V < 0)
      //{
      //  std::cout << "Errore" << '\n';
      //  break;
      //}
    }
  }
}