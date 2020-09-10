#include <benchmark/benchmark.h>

#include <algorithm>
#include <random>
#include <ranges>
#include <string>
#include <vector>

namespace {

auto create_ints(int n) {
  auto v = std::vector<int>(n);
  std::iota(v.begin(), v.end(), 0);
  return v;
}

void bm_sort(benchmark::State& state) {
  const auto n = state.range(0);
  auto r = create_ints(n);
  for (auto _ : state) {
    state.PauseTiming();
    std::random_shuffle(r.begin(), r.end());
    state.ResumeTiming();
    std::ranges::sort(r);
  }
}

void bm_median(benchmark::State& state) {
  const auto n = state.range(0);
  auto r = create_ints(n);
  for (auto _ : state) {
    state.PauseTiming();
    std::random_shuffle(r.begin(), r.end());
    state.ResumeTiming();
    auto middle = r.begin() + r.size() / 2;
    std::nth_element(r.begin(), middle, r.end());
    auto median = *middle;
    benchmark::DoNotOptimize(median);
  }
}

void bm_partial_sort(benchmark::State& state) {
  const auto n = state.range(0);
  auto r = create_ints(n);
  for (auto _ : state) {
    state.PauseTiming();
    std::random_shuffle(r.begin(), r.end());
    state.ResumeTiming();
    std::partial_sort(r.begin(), r.begin() + n / 10, r.end());
  }
}

} // namespace

BENCHMARK(bm_sort)->Arg(10'000'000)->Unit(benchmark::kMillisecond);
BENCHMARK(bm_median)->Arg(10'000'000)->Unit(benchmark::kMillisecond);
BENCHMARK(bm_partial_sort)->Arg(10'000'000)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();