#include "infection.hpp"

namespace epidemic
{
  //std::vector<State> Infection::states() const { return m_data; }

  int Infection::s() const { return m_data.back().S; }

  int Infection::m() const { return m_data.back().M; }

  int Infection::r() const { return m_data.back().R; }

  int Infection::v() const { return m_data.back().V; }

  State Infection::get_state(int day) const { return m_data[day]; };

  std::vector<int> Infection::get_S_vector() const{
    int const N = m_data.size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i)
    {
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

    for (int i = 0; i < N; ++i)
    {
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

    for (int i = 0; i < N; ++i)
    {
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

    for (int i = 0; i < N; ++i)
    {
      int num_m = m_data[i].M;
      result.push_back(num_m);
    }
    assert(m_data.size() == result.size());
    return result;
  }

  void Infection::evolve(double beta, double gamma, int no_vax, double vel_vax, double eff_vax) {

    double const h = 1;

    State support;

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

      m_data.push_back(support);
    }
  }

  void Infection::RK4(double beta, double gamma, int no_vax, double vel_vax, double eff_vax) {

    float const h = 1; // step size

    State support;

    for (int i = 1; i < m_time_indays; ++i) {
      //  if (s() + m() + r() + v() < m_N) {
      //    ++analysis.back().S;
      //  }
      //  if (s() + m() + r() + v() > m_N) {
      //    --analysis.back().S;
      //  }

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

      m_data.push_back(support);

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
    int const N = m_data[0].S + m_data[0].M + m_data[0].R + m_data[0].V;
    int const width = std::log10(N) + 4;

    std::cout << '|' << " day   " << std::string(floor(width / 2) - 1, ' ') << 'S' << std::string(floor(width / 2) - 1, ' ')
              << std::string(floor(width / 2) - 1, ' ') << 'M' << std::string(floor(width / 2) - 1, ' ')
              << std::string(floor(width / 2) - 1, ' ') << 'R' << std::string(floor(width / 2) - 1, ' ')
              << std::string(floor(width / 2) - 1, ' ') << 'V' << std::string(floor(width) - 2, ' ')
              << '|' << '\n';

    for (int i = 0; i < m_time_indays; i++) {
      std::cout << '|' << std::string(2, ' ') << i + 1 << ')' << std::string(4 - std::log10(i + 1.5), ' ')
                << m_data[i].S
                << std::string(width - count_digit(m_data[i].S), ' ') << m_data[i].M
                << std::string(width - count_digit(m_data[i].M), ' ') << m_data[i].R
                << std::string(width - count_digit(m_data[i].R), ' ') << m_data[i].V
                << std::string(width - count_digit(m_data[i].V), ' ') << '|' << '\n';
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

    auto f = matplot::figure();

    f->position({0, 0, 800, 800});
    f->size(800, 800);
    f->draw();

    // S()
    auto ax1 = matplot::nexttile();
    matplot::bar(ax1, S, 0.3)->face_color("blue");
    matplot::title("{/:Italic Susceptible", "blue");
    matplot::ylabel("");
    matplot::xtickformat("%g");
    matplot::xtickangle(0);

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
    matplot::xtickangle(0);

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
    matplot::xtickangle(0);

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
    matplot::xtickangle(0);

    ax4->font_size(10);
    ax4->y_axis().label_font_size(10);
    ax4->y_axis().label_weight("italic");

    matplot::box(false);
    matplot::grid(true);
    ax4->minor_grid(true);

    matplot::sgtitle("HISTOGRAMS", "black");
    matplot::gcf()->title_font_size_multiplier(2.0);

    matplot::save("infection.jpg");
    
    matplot::show();
  }
}