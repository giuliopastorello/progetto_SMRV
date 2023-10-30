#include "matplot/matplot.h"

int main()
{
    std::vector<int> S = [0,1,2,3,4,5,6,7,8,9,10];
    std::vector<int> M = [0,1,2,3,4,5,6,7,8,9,10];
    std::vector<int> R = [0,1,2,3,4,5,6,7,8,9,10];
    std::vector<int> V = [0,1,2,3,4,5,6,7,8,9,10];

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
