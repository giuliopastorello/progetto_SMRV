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

  SUBCASE("testing evolve errors"){
    int beta = 0.5; 
    int gamma = 0.5;
    int alfa = 0.01;
    World world(grid_size);
    initial_random(world, 10, 8);
    CHECK_THROWS(evolve(world, -3, gamma, alfa));
    CHECK_THROWS(evolve(world, beta, 4, alfa));
    CHECK_THROWS(evolve(world, beta, gamma, 15));
  }

  SUBCASE("testing move condition"){
    World world(grid_size);
    world.Set_cell(Cell::I, 0, 0);
    world.Set_cell(Cell::I, 1, 0);
    world.Set_cell(Cell::I, 0, 1);
    world.Set_cell(Cell::I, 2, 2);
    world.Set_cell(Cell::I, 2, 1);
    world.Set_cell(Cell::I, 1, 2);
    world.Set_cell(Cell::I, 2, 0);
    world.Set_cell(Cell::I, 0, 2);
    world.Set_cell(Cell::S, 1, 1);

    CHECK(move_condition(world, 1, 1) == false);
    CHECK(move_condition(world, 2, 2) == true);

    world.Set_cell(Cell::Dead, 3 ,3);

    CHECK(move_condition(world, 3, 3) == false);
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
