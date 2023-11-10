#include "display.hpp"
namespace epidemic {

    int count_digit(int n) {
        return std::log10(n) + 1;
    }

    void print(Infection const &infection) {

        system("clear");
        int const N = infection.get_state(0).S + infection.get_state(0).M + infection.get_state(0).R + infection.get_state(0).V;
        int const width = std::log10(N) + 4;

        std::cout << '|' << termcolor::bright_cyan << " day   " << termcolor::reset << std::string(floor(width / 2) - 1, ' ') << termcolor::bright_blue << 'S' << termcolor::reset << std::string(floor(width / 2) - 1, ' ')
                  << std::string(floor(width / 2) - 1, ' ') << termcolor::red << 'M' << termcolor::reset << std::string(floor(width / 2) - 1, ' ')
                  << std::string(floor(width / 2) - 1, ' ') << termcolor::green << 'R' << termcolor::reset << std::string(floor(width / 2) - 1, ' ')
                  << std::string(floor(width / 2) - 1, ' ') << termcolor::yellow << 'V' << termcolor::reset << std::string(floor(width) - 2, ' ')
                  << '|' << '\n';

        for (int i = 0; i < infection.get_days(); i++) {
            
            std::cout << '|' << std::string(2, ' ') << i + 1 << ')' << std::string(4 - std::log10(i + 1.5), ' ')
                      << infection.get_state(i).S
                      << std::string(width - count_digit(infection.get_state(i).S), ' ') << infection.get_state(i).M
                      << std::string(width - count_digit(infection.get_state(i).M), ' ') << infection.get_state(i).R
                      << std::string(width - count_digit(infection.get_state(i).R), ' ') << infection.get_state(i).V
                      << std::string(width - count_digit(infection.get_state(i).V), ' ') << '|' << '\n';
        }
    }

    void graph(Infection const &infection) {
        std::vector<int> S = infection.get_S_vector();
        std::vector<int> M = infection.get_M_vector();
        std::vector<int> R = infection.get_R_vector();
        std::vector<int> V = infection.get_V_vector();

        auto f = matplot::figure();

        f->position({0, 0, 800, 800});
        f->size(1000, 800);

        // S
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

        // I
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

        // R
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

        // V
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

        matplot::show();
        matplot::save("infection.png");

        matplot::wait();
    }
}