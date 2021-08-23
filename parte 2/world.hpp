#ifndef LIFE_WORLD_HPP
#define LIFE_WORLD_HPP

#include <cassert>
#include <vector>

namespace covid {

    enum class Cell : char { S, M, R, V};

class World 
{
    using Grid = std::vector<Cell>;
    int m_side;
    Grid m_grid;

  public:
    World(int n) : m_side(n), m_grid(m_side * m_side, Cell::S)
    {    
    }
    
 
 
};

}

#endif