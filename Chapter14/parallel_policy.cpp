#include <algorithm>
#include <numeric>
#include <execution>
#include <string>
#include <vector>

#include <gtest/gtest.h>

TEST(ParllelPolicy, DataRaceWithForEach) {
  auto v = std::vector<std::string>{"Ada", "APL" /*...*/};
  auto tot_size = 0ul;
  std::for_each(std::execution::par, v.begin(), v.end(), [&](const auto& s) {
    tot_size += s.size(); // Undefined behavior, data race!
  });
}

TEST(ParllelPolicy, Reduce) {

  auto v = std::vector<std::string>{"Ada", "APL" /*...*/};
  auto tot_size = std::reduce(std::execution::par, v.begin(), v.end(), 0ul,
                              [](auto i, const auto& s) {
                                return i + s.size(); // OK! Thread safe
                              });
}