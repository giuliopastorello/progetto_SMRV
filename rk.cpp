#include <math.h>

#include "infection.hpp"

void Infection::RK4(double beta, double gamma) {
  int const h = 2;  // step size
  //double const beta = 0.056;
  //double const gamma = 0.045;
  int const eta = 118292;    // no vax
  double const mu = 0.05;   // velocità vaccino
  double const xi = 0.839;  // efficacia vaccino
  for (int i = 0; i < m_duration_analysis_indays; ++i) {
  //  if (s() + m() + r() + v() < m_N) {
  //    ++analysis.back().S;
  //  }
  //  if (s() + m() + r() + v() > m_N) {
  //    --analysis.back().S;
  //  }

    double n1 =
        mu * (v() / xi) * (1 - v() / (xi * (m_N - eta)));
    double k1 = -beta / m_N * s() * m() - n1;
    double l1 = beta / m_N * s() * m() - gamma * m();
    double m1 = gamma * m();

    double n2 = mu * ((v() + n1 * h / 2) / xi) *
                (1 - ((v() + n1 * h / 2) / (xi * (m_N - eta))));
    double k2 = -beta / m_N * (s() + k1 * h / 2) * (m() + l1 * h / 2) - n2;
    double l2 = beta / m_N * (s() + k1 * h / 2) * (m() + l1 * h / 2) -
                gamma * (m() + l1 * h / 2);
    double m2 = gamma * (m() + l1 * h / 2);

    double n3 = mu * ((v() + n2 * h / 2) / xi) *
                (1 - ((v() + n2 * h / 2) / (xi * (m_N - eta))));
    double k3 = -beta / m_N * (s() + k2 * h / 2) * (m() + l2 * h / 2) - n3;
    double l3 = beta / m_N * (s() + k2 * h / 2) * (m() + l2 * h / 2) -
                gamma * (m() + l2 * h / 2);
    double m3 = gamma * (m() + l2 * h / 2);

    double n4 = mu * ((v() + n3 * h) / xi) *
                (1 - ((v() + n3 * h) / (xi * (m_N - eta))));
    double k4 = -beta / m_N * (s() + k3 * h) * (m() + l3 * h) - n4;
    double l4 =
        beta / m_N * (s() + k3 * h) * (m() + l3 * h) - gamma * (m() + l3 * h);
    double m4 = gamma * (m() + l3 * h);

    support.V = round(v() + h / 6 * (n1 + 2 * n2 + 2 * n3 + n4));
    support.S = round(s() + h / 6 * (k1 + 2 * k2 + 2 * k3 + k4));
    support.M = round(s() + h / 6 * (l1 + 2 * l2 + 2 * l3 + l4));
    support.R = round(r() + h / 6 * (m1 + 2 * m2 + 2 * m3 + m4));

    analysis.push_back(support);

    if (support.S < 0 || support.M < 0 || support.R < 0 || support.V < 0) {
      std::cout << "Errore" << '\n';
      break;
    }
  }
}
