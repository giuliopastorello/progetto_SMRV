#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "infection.hpp"

void print(std::vector<State> const &states);

std::vector<int> get_S(std::vector<State> const &states);
std::vector<int> get_I(std::vector<State> const &states);
std::vector<int> get_R(std::vector<State> const &states);
std::vector<int> get_D(std::vector<State> const &states);
 
void graph(std::vector<State> const &states);

#endif