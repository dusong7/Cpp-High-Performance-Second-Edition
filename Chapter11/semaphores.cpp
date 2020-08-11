#include <array>
#include <gtest/gtest.h>
#include <iostream>
#include <semaphore>
#include <optional>
#include <string>
#include <thread>

namespace {

template <class T, int N> class BoundedBuffer {
  std::array<T, N> buf_;
  std::size_t read_pos_{};
  std::size_t write_pos_{};
  std::mutex m_;
  std::counting_semaphore<N> n_empty_slots_{N}; // New
  std::counting_semaphore<N> n_full_slots_{0};  // New

public:
  void push(const T& item) {
    // Take one of the empty slots (might block)
    n_empty_slots_.acquire(); // New
    {
      auto lock = std::unique_lock{m_};
      buf_[write_pos_] = item; // Copy
      write_pos_ = (write_pos_ + 1) % N;
    }
    // Increment and signal that there is one more full slot
    n_full_slots_.release(); // New
  }
  void push(T&& item) {
    // Take one of the empty slots (might block)
    n_empty_slots_.acquire(); // New
    {
      auto lock = std::unique_lock{m_};
      buf_[write_pos_] = std::move(item);
      write_pos_ = (write_pos_ + 1) % N;
    }
    // Increment and signal that there is one more full slot
    n_full_slots_.release(); // New
  }

  auto pop() {
    // Take one of the full slots (might block)
    n_full_slots_.acquire(); // New
    auto item = std::optional<T>{};
    {
      auto lock = std::unique_lock{m_};
      item = std::move(buf_[read_pos_]);
      read_pos_ = (read_pos_ + 1) % N;
    }
    // Increment and signal that there is one more empty slot
    n_empty_slots_.release(); // New
    return std::move(*item);
  }
};

} // namespace

TEST(Semaphores, BoundedBuffer) {

  auto buffer = BoundedBuffer<std::string, 5>{};
  auto sentinel = std::string{""};

  auto producer = std::thread{[&]() {
    buffer.push("A");
    buffer.push("B");
    buffer.push("C");
    buffer.push(sentinel);
  }};

  auto consumer = std::thread{[&]() {
    for (;;) {
      auto s = buffer.pop();
      if (s != sentinel) {
        std::cout << "Got: " << s << '\n';
      } else {
        break;
      }
    }
  }};

  producer.join();
  consumer.join();
}
