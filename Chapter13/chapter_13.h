#pragma once 

#if defined(_MSC_VER) || defined(__clang__)
#include <experimental/coroutine>
namespace std {
    using std::experimental::coroutine_handle;
    using std::experimental::suspend_always;
    using std::experimental::suspend_never;
}
#else
#include <coroutine>
#endif