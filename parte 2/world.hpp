#ifndef LIFE_WORLD_HPP
#define LIFE_WORLD_HPP

#include <cassert>
#include <vector>

namespace covid {

    enum class Cell : char { S, I, R, V};

class World 
{
    using Grid = std::vector<Cell>;
    int m_side;
    Grid m_grid;

  public:
    World(int n);
    int side() const
    {
        return m_side;
    }
    Cell const& cell(int r, int c) const noexcept 
    {
        auto const i = (r + m_side) % m_side; //row index
        auto const j = (c + m_side) % m_side; //column index
        assert(i >= 0 && i < m_side && j >= 0 && j < m_side);
        auto const index = i * m_side + j;
        assert(index >= 0 && index < static_cast<int>(m_grid.size()));
        return m_grid[index];
    }
    Cell& cell(int r, int c) noexcept
    {
        auto const i = (r + m_side) % m_side;
        auto const j = (c + m_side) % m_side;
        assert(i >= 0 && i < m_side && j >= 0 && j < m_side);
        auto const index = i * m_side + j;
        assert(index >= 0 && index < static_cast<int>(m_grid.size()));
        return m_grid[index];
    }  

    friend bool operator==(World const& l, World const& r)
    {
        return l.m_grid == r.m_grid;
    }
};

inline int neighbours_infected(World const& world, int r, int c)
{
    ;
}
}

#endif