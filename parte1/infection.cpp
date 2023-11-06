#include "infection.hpp"
#include <cmath>
#include <cassert>
#include <matplot/matplot.h>


namespace epidemic
{
  std::vector<State> Infection::states() const { return analysis; }

  int Infection::s() const { return analysis.back().S; }

  int Infection::m() const { return analysis.back().M; }

  int Infection::r() const { return analysis.back().R; }

  int Infection::v() const { return analysis.back().V; }

  State Infection::get_state(int day) const { return analysis[day]; };

  std::vector<int> Infection::get_S_vector() const{
    int const N = Infection::states().size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i)
    {
      int num_s = Infection::states()[i].S;
      result.push_back(num_s);
    }
    assert(Infection::states().size() == result.size());
    return result;
  }

  std::vector<int> Infection::get_R_vector() const {
    int const N = Infection::states().size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i)
    {
      int num_r = Infection::states()[i].R;
      result.push_back(num_r);
    }
    assert(Infection::states().size() == result.size());
    return result;
  }

  std::vector<int> Infection::get_V_vector() const {
    int const N = Infection::states().size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i)
    {
      int num_v = Infection::states()[i].V;
      result.push_back(num_v);
    }
    assert(Infection::states().size() == result.size());
    return result;
  }

  std::vector<int> Infection::get_M_vector() const {
    int const N = Infection::states().size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i)
    {
      int num_m = Infection::states()[i].M;
      result.push_back(num_m);
    }
    assert(Infection::states().size() == result.size());
    return result;
  }

  void Infection::evolve(double beta, double gamma) {
    double const h = 1;
    int const no_vax = 118292;   // eta
    double const vel_vax = 0.05;  // mu
    double const eff_vax = 0.839; // efficacia vaccino

    for (int i = 1; i < m_time_indays; ++i) {
      double delV = vel_vax * (v() / eff_vax) *
                    (1 - v() / (eff_vax * (m_N - no_vax)));

      double delS = -beta * (s() / m_N) * m() - vel_vax * (v() / eff_vax) *
                            (1 - v() / (eff_vax * (m_N - no_vax)));

      double delR = gamma * m();

      support.V = round(v() + h * delV);
      support.S = round(s() + h * delS);
      support.R = round(r() + h * delR);
      support.M = round(m() - h * (delV + delS + delR));

      analysis.push_back(support);
    }
  }

  void Infection::RK4(double beta, double gamma) {

    float const h = 1; // step size
    // double const beta = 0.056;
    // double const gamma = 0.045;
    int const eta = 118292;  // no vax
    double const mu = 0.05;  // velocità vaccino
    double const xi = 0.839; // efficacia vaccino
    
    for (int i = 1; i < m_time_indays; ++i) {
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

  int count_digit(int n) {
    return std::log10(n) + 1;
  }

  void Infection::print() const {
    int const N = analysis[0].S + analysis[0].M + analysis[0].R + analysis[0].V;
    int const width = std::log10(N) + 4;

    std::cout << '|' << " day   " << std::string(floor(width / 2) - 1, ' ') << 'S' << std::string(floor(width / 2) - 1, ' ')
              << std::string(floor(width / 2) - 1, ' ') << 'M' << std::string(floor(width / 2) - 1, ' ')
              << std::string(floor(width / 2) - 1, ' ') << 'R' << std::string(floor(width / 2) - 1, ' ')
              << std::string(floor(width / 2) - 1, ' ') << 'V' << std::string(floor(width) - 2, ' ')
              << '|' << '\n';

    for (int i = 0; i < m_time_indays; i++) {
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

  void Infection::graph() {

    std::vector<int> S = Infection::get_S_vector();
    std::vector<int> M = Infection::get_M_vector();
    std::vector<int> R = Infection::get_R_vector();
    std::vector<int> V = Infection::get_V_vector();

    auto f = matplot::figure(true);

    // S()
    auto ax1 = matplot::nexttile();
    matplot::bar(ax1, S, 0.3)->face_color("blue");
    matplot::title("{/:Italic Susceptible",
                   "blue");
    matplot::ylabel("");
    matplot::xtickformat("%g");
    matplot::xtickangle(45);

    ax1->font_size(10);
    ax1->y_axis().label_font_size(10);
    ax1->y_axis().label_weight("italic");

    matplot::box(false);
    matplot::grid(true);
    ax1->minor_grid(true);

    // Infected
    auto ax2 = matplot::nexttile();
    matplot::bar(ax2, M, 0.3)->face_color("red");
    matplot::title("{/:Italic Infected", "red");
    matplot::ylabel("");
    matplot::xtickformat(" %g");
    matplot::xtickangle(45);

    ax2->font_size(10);
    ax2->y_axis().label_font_size(10);
    ax2->y_axis().label_weight("italic");

    matplot::box(false);
    matplot::grid(true);
    ax2->minor_grid(true);

    // Recovered
    auto ax3 = matplot::nexttile();
    matplot::bar(ax3, R, 0.3)->face_color("green");
    matplot::title("{/:Italic Recovered", "green");
    matplot::ylabel("");
    matplot::xtickformat("%g");
    matplot::xtickangle(45);

    ax3->font_size(10);
    ax3->y_axis().label_font_size(10);
    ax3->y_axis().label_weight("italic");

    matplot::box(false);
    matplot::grid(true);
    ax3->minor_grid(true);

    // Dead
    auto ax4 = matplot::nexttile();
    matplot::bar(ax4, V, 0.3)->face_color("black");
    matplot::title("{/:Italic Vaccinated", "black");
    matplot::ylabel("");
    matplot::xtickformat("%g");
    matplot::xtickangle(45);

    ax4->font_size(10);
    ax4->y_axis().label_font_size(10);
    ax4->y_axis().label_weight("italic");

    matplot::box(false);
    matplot::grid(true);
    ax4->minor_grid(true);

    matplot::sgtitle("HISTOGRAMS", "black");
    matplot::gcf()->title_font_size_multiplier(2.0);

    matplot::show();
    matplot::save("infection.jpg");

  }
}