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
    int side() const
    {
        return m_side;
    }
    Cell const& cell(int r, int c) const noexcept 
    {
        auto const i = (r + m_side) % m_side;
        auto const j = (c + m_side) % m_side;
        assert(i >= 0 && i < m_side && j >= 0 && j < m_side);
        auto const index = i * m_side + j;
        assert(index >= 0 && index < static_cast<int>(m_grid.size()));
        return m_grid[index];
    }
 
};

}

#endif