#if defined(__cpp_lib_jthread)
#include <iostream>
#include <jthread>
#include <gtest/gtest.h>


void print(std::stop_token stoken) {
  while (!stoken.stop_requested()) { 
    std::cout << std::this_thread::get_id() << '\n';
    std::this_thread::sleep_for(std::chrono::seconds{1});
  }
  std::cout << "Stop requested\n";
}

TEST(StopToken, BasicExample) {
  auto joinable_thread = std::jthread(print);
  std::cout << "main: goes to sleep\n";
  std::this_thread::sleep_for(std::chrono::seconds{5});
  std::cout << "main: request jthread to stop\n";
  joinable_thread.request_stop();                                                                                                                                                                         
}
#endif 