//#include "display.hpp"

#include <matplot/matplot.h> //problema include creato per averlo spostato in parte2/

#include <cassert>
#include "infection.hpp"

namespace epidemic
{

  std::vector<int> Infection::get_S()
  {
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

  std::vector<int> Infection::get_M()
  {
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

  std::vector<int> Infection::get_R()
  {
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

  std::vector<int> Infection::get_V()
  {
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

  void Infection::graph()
  {

    std::vector<int> S = Infection::get_S();
    std::vector<int> M = Infection::get_M();
    std::vector<int> R = Infection::get_R();
    std::vector<int> V = Infection::get_V();

    auto f = matplot::figure(true);
    //f->matplot::backend()->matplot::output("file.svg");

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
    matplot::bar(ax2, M, 0.3)->face_color("red");
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
    matplot::bar(ax4, V, 0.3)->face_color("black");
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

    matplot::show();
  }
}
