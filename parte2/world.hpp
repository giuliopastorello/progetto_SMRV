#ifndef LIFE_WORLD_HPP
#define LIFE_WORLD_HPP

#include <algorithm>
#include <cassert>
#include <vector>

namespace covid {

    enum class Cell : char { S, I, R};

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
        auto const i = (r + m_side) % m_side; //column index
        auto const j = (c + m_side) % m_side; //row index
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

    int count_s() const {
        return std::count(m_grid.begin(), m_grid.end(), Cell::S);
    }

    int count_i() const {
        return std::count(m_grid.begin(), m_grid.end(), Cell::I);
    }

    int count_r() const {
        return std::count(m_grid.begin(), m_grid.end(), Cell::R);
    }
};

}
#endif