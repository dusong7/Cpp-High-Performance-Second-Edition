// Compile using GCC: g++ -std=c++2a -O3 -lpthread -Wall

#include <atomic>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <random>

namespace SpinLock {

class SimpleMutex {
  std::atomic_flag is_locked{}; // Cleared by default
public:
  auto lock() noexcept {
    while (is_locked.test_and_set()) {
      while (is_locked.test())
        ; // Spin here
    }
  }
  auto unlock() noexcept { is_locked.clear(); }
};

TEST(Atomics, SimpleSpinLock) {

  constexpr auto n = 1000000;
  auto counter = 0; // Counter will be protected by mutex
  auto counter_mutex = SimpleMutex{};

  auto increment_counter = [&] {
    for (int i = 0; i < n; ++i) {
      counter_mutex.lock();
      ++counter;
      counter_mutex.unlock();
    }
  };

  auto t1 = std::thread{increment_counter};
  auto t2 = std::thread{increment_counter};

  t1.join(); // Or use std::jthread
  t2.join();

  std::cout << counter << '\n';
  // If we don't have a data race, this assert should hold:
  ASSERT_EQ(n_times * 2, counter);
}

} // namespace SpinLock

namespace WaitAndNotify {

class SimpleMutex {
  std::atomic_flag is_locked{};

public:
  auto lock() noexcept {
    while (is_locked.test_and_set())
      is_locked.wait(true); // Don’t spin, wait
  }

  auto unlock() noexcept {
    is_locked.clear();
    is_locked.notify_one(); // Notify blocked thread
  }
};

TEST(Atomics, WaitAndNotify) {

  constexpr auto n = 1000000;
  auto counter = 0; // Counter will be protected by mutex
  auto counter_mutex = SimpleMutex{};

  auto increment_counter = [&] {
    for (int i = 0; i < n; ++i) {
      counter_mutex.lock();
      ++counter;
      counter_mutex.unlock();
    }
  };

  auto t1 = std::thread{increment_counter};
  auto t2 = std::thread{increment_counter};

  t1.join(); // Or use std::jthread
  t2.join();

  std::cout << counter << '\n';
  // If we don't have a data race, this assert should hold:
  ASSERT_EQ(n_times * 2, counter);
}

} // namespace WaitAndNotify

namespace AtomicReferences {

auto random_int(int min, int max) {
  // One engine instance per thread
  thread_local static auto engine =
      std::default_random_engine{std::random_device{}()};

  auto dist = std::uniform_int_distribution<>{min, max};
  return dist(engine);
}

struct Stats {
  int heads{};
  int tails{};
};

std::ostream& operator<<(std::ostream& os, const Stats& s) {
  os << "heads: " << s.heads << ", tails: " << s.tails;
  return os;
}

void filp_coin(std::size_t n, Stats& outcomes) {
  auto flip = [&outcomes](auto n) {
    auto heads = std::atomic_ref<int>{outcomes.heads};
    auto tails = std::atomic_ref<int>{outcomes.tails};
    for (auto i = 0u; i < n; ++i) {
      random_int(0, 1) == 0 ? ++heads : ++tails;
    }
  };
  auto t1 = std::jthread{flip, n / 2};       // First half
  auto t2 = std::jthread{flip, n - (n / 2)}; // The rest
}

TEST(Atomics, AtomicReferences) {
  auto stats = Stats{};
  filp_coin(5000, stats); // Flip 5000 times
  std::cout << stats << '\n';
  ASSERT_EQ(5000, (stats.tails + stats.heads));
}

} // namespace AtomicReferences
