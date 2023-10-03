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

  for (State const& state : states) {
    simulation.print(state);
  }

    } 

  //  std::vector<int> get_S(Infection const &simulation) {
  //  
  //  std::vector<State> states = simulation.states();
  //  
  //  int const N = states.size();
  //  std::vector<int> result{};
  //  result.reserve(N);
//
  //  for (State const& state : states) {
  //    int num_s = virus.state().S;
  //    result.push_back(num_s);
  //  }
  //  assert(states.size() == result.size());
  //  return result;
//}

} 
