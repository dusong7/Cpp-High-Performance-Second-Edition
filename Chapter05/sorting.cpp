#include <gtest/gtest.h>

#include <algorithm>
#include <ranges>

TEST(Sorting, Sort) {
  auto v = std::vector{5, 2, 1, 7, 3, 0, 4};
  std::ranges::sort(v);

  ASSERT_TRUE(std::ranges::is_sorted(v));
}

TEST(Sorting, NthElement) {
  auto v = std::vector{5, 2, 1, 7, 3, 0, 4};
  auto it = v.begin() + v.size() / 2;

  std::nth_element(v.begin(), it, v.end());

  // it is the only sorted element
  ASSERT_EQ(*it, 3);
  // Max element in the left subrange
  auto left = *std::max_element(v.begin(), it);
  // Min element in the right subrange
  auto right = *std::min_element(it+1, v.end());
  ASSERT_TRUE(left <= right);
}

TEST(Sorting, PartialSort) {
  auto v = std::vector{5, 2, 1, 7, 3, 0, 4};
  auto it = v.begin() + v.size() / 2;

  std::partial_sort(v.begin(), it, v.end());

  ASSERT_TRUE(std::is_sorted(v.begin(), it));
}