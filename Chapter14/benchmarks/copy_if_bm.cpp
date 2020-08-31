#include "../copy_if.h"

#include <algorithm>
#include <benchmark/benchmark.h>
#include <cassert>
#include <future>
#include <numeric>
#include <tuple>
#include <vector>

namespace {

// Test 1
// Naive vs std::transform()
//

// Inexpensive predicate
struct IsOdd {
  bool operator()(unsigned v) { return (v % 2) == 1; }
};

// Expensive predicate
struct IsPrime {
  bool operator()(unsigned v) {
    if (v < 2)
      return false;
    if (v == 2)
      return true;
    if (v % 2 == 0)
      return false;
    for (unsigned i = 3; (i * i) <= v; i += 2) {
      if ((v % i) == 0) {
        return false;
      }
    }
    return true;
  }
};
auto setup_fixture(int n) {
  auto src = std::vector<float>(n);
  std::iota(src.begin(), src.end(), 1.0f); // "src" goes from 1.0 to n
  auto dst = std::vector<float>(src.size());
  return std::tuple{src, dst};
}

template <typename Pred> void bm_std_copy_if(benchmark::State& state) {
  auto [src, dst] = setup_fixture(100'000'000);
  auto pred = Pred{};
  for (auto _ : state) {
    auto new_end = std::copy_if(src.begin(), src.end(), dst.begin(), pred);
    benchmark::DoNotOptimize(new_end);
  }
}

template <class Pred> void bm_par_copy_if_sync(benchmark::State& state) {
  auto [src, dst] = setup_fixture(100'000'000);
  auto pred = Pred{};
  for (auto _ : state) {
    auto new_end =
        par_copy_if_sync(src.begin(), src.end(), dst.begin(), pred, 100'000);
    benchmark::DoNotOptimize(new_end);
  }
}

template <class Pred> void bm_par_copy_if_split(benchmark::State& state) {
  auto [src, dst] = setup_fixture(100'000'000);
  auto pred = Pred{};
  for (auto _ : state) {
    auto new_end =
        par_copy_if_split(src.begin(), src.end(), dst.begin(), pred, 100'000);
    benchmark::DoNotOptimize(new_end);
  }
}

void CustomArguments(benchmark::internal::Benchmark* b) {
  b->MeasureProcessCPUTime()->UseRealTime()->Unit(benchmark::kMillisecond);
}

} // namespace

BENCHMARK_TEMPLATE(bm_std_copy_if, IsOdd)->Apply(CustomArguments);
BENCHMARK_TEMPLATE(bm_par_copy_if_split, IsOdd)->Apply(CustomArguments);
BENCHMARK_TEMPLATE(bm_par_copy_if_sync, IsOdd)->Apply(CustomArguments);

BENCHMARK_TEMPLATE(bm_std_copy_if, IsPrime)->Apply(CustomArguments);
BENCHMARK_TEMPLATE(bm_par_copy_if_split, IsPrime)->Apply(CustomArguments);
BENCHMARK_TEMPLATE(bm_par_copy_if_sync, IsPrime)->Apply(CustomArguments);

BENCHMARK_MAIN();