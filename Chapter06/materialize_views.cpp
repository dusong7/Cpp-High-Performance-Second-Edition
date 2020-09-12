#include <gtest/gtest.h>

#include <algorithm>
#include <list>
#include <ranges>
#include <string>
#include <vector>

// Materialize the range r into a std::vector
// See also https://timur.audio/how-to-make-a-container-from-a-c20-range
auto to_vector(auto&& r) {
  std::vector<std::ranges::range_value_t<decltype(r)>> v;
  if constexpr (std::ranges::sized_range<decltype(r)>) {
    v.reserve(std::ranges::size(r));
  }
  std::ranges::copy(r, std::back_inserter(v));
  return v;
}

TEST(MaterializeViews, IntListToStringVector) {

  auto ints = std::list{2, 3, 4, 2, 1};
  auto r =
      ints | std::views::transform([](auto i) { return std::to_string(i); });
  auto strings = to_vector(r);

  auto result = std::vector<std::string>{"2", "3", "4", "2", "1"};
  ASSERT_EQ(strings, result);
}

TEST(MaterializeViews, IteratorQualityNotPreserved) {

  auto vec = std::vector{4, 2, 7, 1, 2, 6, 1, 5};
  auto is_odd = [](auto i) { return i % 2 == 1; };
  auto odd_numbers = vec | std::views::filter(is_odd);

  // Doesn't compile, sort() requires random access iterators
  // std::ranges::sort(odd_numbers);  // Doesn't compile

  // OK, odd_number is materialized into a vector
  auto v = to_vector(odd_numbers);
  std::ranges::sort(v);
}

TEST(MaterializeViews, IteratorQualityPreserved) {
  auto vec = std::vector<int>{4, 2, 7, 1, 2, 6, 1, 5};
  auto first_half = vec | std::views::take(vec.size() / 2);
  std::ranges::sort(first_half);
  // vec is now 1, 2, 4, 7, 2, 6, 1, 5

  auto result = std::vector{1, 2, 4, 7, 2, 6, 1, 5};
  ASSERT_EQ(vec, result);
}