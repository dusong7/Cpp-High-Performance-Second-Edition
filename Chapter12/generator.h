
#include <exception>
#include <utility>

#ifdef _MSC_VER
#include <experimental/coroutine>
namespace stdx = std::experimental;
#else
#include <coroutine>
//namespace stdx = std;
namespace cons = std;
#endif

template <typename T>
struct Generator {

private:
  struct Promise {
  T value_;
  auto get_return_object() -> Generator {
    using Handle = cons::coroutine_handle<Promise>;
    return Generator{Handle::from_promise(*this)};
  }
  auto initial_suspend() { return cons::suspend_always{}; }
  auto final_suspend() noexcept { return cons::suspend_always{}; }
  void return_void() {}
  void unhandled_exception() { throw; }
  auto yield_value(T&& value) {
    value_ = std::move(value);
    return cons::suspend_always{};
  }
   auto yield_value(const T& value) {
    value_ = value;
    return cons::suspend_always{};
  }
};

struct Sentinel {};

struct Iterator {

  using iterator_category = std::input_iterator_tag;
  using value_type = T;
  using difference_type = ptrdiff_t;
  using pointer = T*;
  using reference = T&;

  cons::coroutine_handle<Promise> h_;
  Iterator& operator++() {
    h_.resume();
    return *this;
  }
  void operator++(int) { (void)operator++(); }
  T operator*() const { return  h_.promise().value_; }
  pointer operator->() const { return std::addressof(operator*()); }
  bool operator==(Sentinel) const { return h_.done(); }
};

  
  cons::coroutine_handle<Promise> h_;
  explicit Generator(cons::coroutine_handle<Promise> h) : h_(h) {}

public: 
  using promise_type = Promise;

  Generator(Generator&& g) : h_(std::exchange(g.h_, {})) {}
  ~Generator() { if (h_) { h_.destroy();  } }

  auto begin() {
    h_.resume();
    return Iterator{h_};
  }
  auto end() { return Sentinel{}; }
};
