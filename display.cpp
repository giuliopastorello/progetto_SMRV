#include "infection.hpp"
#include "display.hpp"

#include <matplot/matplot.h>

#include "include/termcolor.hpp"

namespace epidemic
{

    void print(Infection const &simulation)
    {
        std::vector<State> states = simulation.states();
        
        int N = states[0].S + states[0].M + states[0].R +
                states[0].V;
        int const width = std::log10(N) + 4;

        // clang-format off
        std::cout << '\n'
                  << '+' << std::string(width - 1, '-') << '+'
                  << std::string(width - 1, '-') << '+' << std::string(width - 1, '-')
                  << '+' << std::string(width, '-') << '+' << '\n'
                  << '|' << 'S' << std::string(width - 2, ' ')
                  << '|' << 'M' << std::string(width - 2, ' ')
                  << '|' << 'R' << std::string(width - 2, ' ')
                  << '|' << 'V' << std::string(width - 1, ' ')
                  << '|' << '\n'
                  << '+' << std::string(width - 1, '-') << '+'
                  << std::string(width - 1, '-') << '+' << std::string(width - 1, '-')
                  << '+' << std::string(width, '-') << '+' << '\n';
        // clang-format off

  for (State const& state : states) {
    simulation.print(state);
  }

    } 
} 
