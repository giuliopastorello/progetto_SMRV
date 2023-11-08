#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "infection.hpp"
#include "../include/termcolor.hpp"

namespace epidemic
{
    int count_digit(int n);

    void print(Infection const &infection);

    void graph(Infection const &infection);
}

#endif