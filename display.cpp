#include "display.hpp"

#include <matplot/matplot.h>

#include <cassert>

namespace epidemic{

std::vector<int> get_S(std::vector<State> const &states) {
  int const N = states.size();
  std::vector<int> result{};
  result.reserve(N);

  for (int i = 0; i < N; ++i) {
    int num_s = states[i].S;
    result.push_back(num_s);
  }
  assert(states.size() == result.size());
  return result;
}

std::vector<int> get_M(std::vector<State> const &states) {
  int const N = states.size();
  std::vector<int> result{};
  result.reserve(N);

  for (int i = 0; i < N; ++i) {
    int num_m = states[i].M;
    result.push_back(num_m);
  }
  assert(states.size() == result.size());
  return result;
}

std::vector<int> get_R(std::vector<State> const &states) {
  int const N = states.size();
  std::vector<int> result{};
  result.reserve(N);

  for (int i = 0; i < N; ++i) {
    int num_r = states[i].R;
    result.push_back(num_r);
  }
  assert(states.size() == result.size());
  return result;
}

std::vector<int> get_V(std::vector<State> const &states) {
  int const N = states.size();
  std::vector<int> result{};
  result.reserve(N);

  for (int i = 0; i < N; ++i) {
    int num_v = states[i].V;
    result.push_back(num_v);
  }
  assert(states.size() == result.size());
  return result;
}

void graph(std::vector<State> const &states){}
}

