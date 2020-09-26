#include <gtest/gtest.h>

#include "./generator.h"

template <typename T>
Generator<T> seq() {
  for (T i = {};; ++i) {
    co_yield i;
  }
}

template <typename T>
Generator<T> take_until(Generator<T>& gen, T value) {
  for (auto&& v : gen) {
    if (v == value) {
      co_return;
    }
    co_yield v;
  }
}

template <typename T>
Generator<T> add(Generator<T>& gen, T adder) {
  for (auto&& v : gen) {
    co_yield v + adder;
  }
}

TEST(GeneratorExamlpe, SeqTakeUntilAdd) {
  auto s = seq<int>();
  auto t = take_until<int>(s, 10);
  auto a = add<int>(t, 3);

  int sum = 0;
  for (auto&& v : a) {
    sum += v;
  }
  ASSERT_EQ(75, sum);
}