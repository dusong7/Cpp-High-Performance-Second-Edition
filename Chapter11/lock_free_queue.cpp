#include <array>
#include <atomic>
#include <cassert>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

template <class T, size_t N> class LockFreeQueue {
public:
  auto size() const noexcept { return size_.load(); }

  // Writer thread (move)
  bool push(T&& t) {
    if (size_.load() == N) {
      return false;
    }
    buffer_[write_pos_] = std::move(t);
    write_pos_ = (write_pos_ + 1) % N;
    size_.fetch_add(1);
    return true;
  }
  // Writer thread (copy)
  bool push(const T& t) {
    if (size_.load() == N) {
      return false;
    }
    buffer_[write_pos_] = t;
    write_pos_ = (write_pos_ + 1) % N;
    size_.fetch_add(1);
    return true;
  }

  // Reader thread
  bool pop(T& value) {
    if (size_.load() == 0) {
      return false;
    }
    value = std::move(buffer_[read_pos_]);
    read_pos_ = (read_pos_ + 1) % N;
    size_.fetch_sub(1);
    return true;
  }

private:
  std::array<T, N> buffer_{};   // Used by both threads
  std::atomic<size_t> size_{0}; // Used by both threads
  size_t read_pos_{0};          // Used by reader thread
  size_t write_pos_{0};         // Used by writer thread
  static_assert(std::atomic<size_t>::is_always_lock_free);
};

constexpr auto max_size = 10000;
constexpr auto done = -1;

TEST(LockFreeQueue, PutAndPop) {
  LockFreeQueue<int, max_size> queue;
  std::vector<int> result;

  std::thread writer{[&queue]() {
    for (auto i = 0; i < 10; ++i) {
      queue.push(int{i});
    }
    queue.push(int{done});
  }};

  std::thread reader{[&queue, &result]() {
    for (;;) {
      auto element = int{};
      if (queue.pop(element)) {
        if (element != done) {
          std::cout << "Got: " << element << '\n';
          result.push_back(element);
        } else {
          break;
        }
      }
    }
  }};

  writer.join();
  reader.join();

  ASSERT_EQ(std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), result);
}
