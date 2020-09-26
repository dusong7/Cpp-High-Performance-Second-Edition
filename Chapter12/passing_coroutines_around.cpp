#include "./resumable.h"

#include <gtest/gtest.h>

#include <limits>
#include <thread>

#ifdef _MSC_VER
#include <experimental/coroutine>
namespace stdx = std::experimental;
#else
#include <coroutine>
//namespace stdx = std;
namespace cons = std;
#endif

namespace {

Resumable coroutine() { // Same as before
  std::cout << "c1 ";
  co_await cons::suspend_always{};
  std::cout << "c2 ";
}

auto coro_factory() { // Create and return a coroutine
  auto res = coroutine();
  return std::move(res);
}

} // namespace

TEST(PassingCoroutinesAround, ResumeFromDifferentThread) {
  auto r = coro_factory();
  r.resume(); // Resume from main

  auto t = std::thread{[r = std::move(r)]() mutable {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);
    r.resume(); // Resume from thread
  }};
  t.join();
}
