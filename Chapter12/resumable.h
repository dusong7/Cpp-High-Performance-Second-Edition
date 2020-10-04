
//__cpp_lib_coroutine
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

//#include <experimental/coroutine>
//namespace stdx = std::experimental;

class Resumable { // The return object
  struct Promise {
    auto get_return_object() {
      using Handle = cons::coroutine_handle<Promise>;
      return Resumable{Handle::from_promise(*this)};
    }
    auto initial_suspend() { return cons::suspend_always{}; }
    auto final_suspend() noexcept { return cons::suspend_always{}; }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
  };
  cons::coroutine_handle<Promise> h_;
  explicit Resumable(cons::coroutine_handle<Promise> h) : h_{h} {}

public:
  using promise_type = Promise;
  Resumable(Resumable&& r) : h_{std::exchange(r.h_, {})} {}
  ~Resumable() {
    if (h_) {
      h_.destroy();
    }
  }
  bool resume() {
    if (!h_.done()) {
      h_.resume();
    }
    return !h_.done();
  }
};
