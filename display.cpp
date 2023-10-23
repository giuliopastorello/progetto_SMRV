#include "display.hpp"

#include <matplot/matplot.h>

#include <cassert>

namespace epidemic
{

  std::vector<int> get_S(std::vector<State> const &states)
  {
    int const N = states.size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i)
    {
      int num_s = states[i].S;
      result.push_back(num_s);
    }
    assert(states.size() == result.size());
    return result;
  }

  std::vector<int> get_M(std::vector<State> const &states)
  {
    int const N = states.size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i)
    {
      int num_m = states[i].M;
      result.push_back(num_m);
    }
    assert(states.size() == result.size());
    return result;
  }

  std::vector<int> get_R(std::vector<State> const &states)
  {
    int const N = states.size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i)
    {
      int num_r = states[i].R;
      result.push_back(num_r);
    }
    assert(states.size() == result.size());
    return result;
  }

  std::vector<int> get_V(std::vector<State> const &states)
  {
    int const N = states.size();
    std::vector<int> result{};
    result.reserve(N);

    for (int i = 0; i < N; ++i)
    {
      int num_v = states[i].V;
      result.push_back(num_v);
    }
    assert(states.size() == result.size());
    return result;
  }

  void graph(std::vector<State> const &states)
  {

    std::vector<int> const S = get_S(states);
    std::vector<int> const I = get_M(states);
    std::vector<int> const R = get_R(states);
    std::vector<int> const D = get_V(states);

    auto f2 = matplot::figure();
    auto f = matplot::figure();

    // S()
    auto ax1 = matplot::nexttile();
    matplot::bar(ax1, S, 0.3)->face_color("blue");
    matplot::title("{/:Italic Susceptible",
                   "blue");
    matplot::ylabel("Number of people");
    matplot::xtickformat("day %g");
    matplot::xtickangle(45);

    ax1->font_size(10);
    ax1->y_axis().label_font_size(10);
    ax1->y_axis().label_weight("italic");

    matplot::box(false);
    matplot::grid(true);
    ax1->minor_grid(true);

    // Infected
    auto ax2 = matplot::nexttile();
    matplot::bar(ax2, I, 0.3)->face_color("red");
    matplot::title("{/:Italic Infected", "red");
    matplot::ylabel("Number of people");
    matplot::xtickformat("day %g");
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
    matplot::ylabel("Number of people");
    matplot::xtickformat("day %g");
    matplot::xtickangle(45);

    ax3->font_size(10);
    ax3->y_axis().label_font_size(10);
    ax3->y_axis().label_weight("italic");

    matplot::box(false);
    matplot::grid(true);
    ax3->minor_grid(true);

    // Dead
    auto ax4 = matplot::nexttile();
    matplot::bar(ax4, D, 0.3)->face_color("black");
    matplot::title("{/:Italic Dead", "black");
    matplot::ylabel("Number of people");
    matplot::xtickformat("day %g");
    matplot::xtickangle(45);

    ax4->font_size(10);
    ax4->y_axis().label_font_size(10);
    ax4->y_axis().label_weight("italic");

    matplot::box(false);
    matplot::grid(true);
    ax4->minor_grid(true);

    matplot::sgtitle("HISTOGRAMS", "black");
    matplot::gcf()->title_font_size_multiplier(2.0);
  }
}
