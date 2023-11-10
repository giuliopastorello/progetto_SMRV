#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "world.hpp"

#include "../include/doctest.h"

TEST_CASE("Testing Virus") {
    
  using namespace Virus;
  int const grid_size = 5;

  SUBCASE("Testing Throws") {
    int const n = -1;
    CHECK_THROWS(World(n));

    World world(grid_size);
    CHECK_THROWS(initial_random(world, 10, 30));

    World world2(grid_size);
    CHECK_THROWS(initial_random(world2, 10, 0));
  }

  SUBCASE("Testing infected_counter") {
    World world(grid_size);
    world.Set_cell(Cell::I, 1, 0);
    world.Set_cell(Cell::I, 2, 2);
    world.Set_cell(Cell::I, 3, 2);

    int const infected_around = infected_counter(world, 2, 1);

    CHECK(infected_around == 3);
  }

}
