#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "infection.hpp"

namespace epidemic
{
    void print(Infection const &simulation);

    std::vector<int> get_S(std::vector<State> const &simulation);
    std::vector<int> get_I(std::vector<State> const &simulation);
    std::vector<int> get_R(std::vector<State> const &simulation);
    std::vector<int> get_D(std::vector<State> const &simulation);

    //void plot(std::vector<State> const &simulation);
}
#endif