#include <chrono>
#include <ctime>
#include <iostream>
#include <mutex>
#include <stdint.h>
#include <string>
#include <thread>
typedef void (*callback_function)(std::string str);

class RateLimitter {
public:
  // Get new instance of RateLimitter.
  static RateLimitter *New(uint32_t rate, uint32_t burst);

  // Reserve for tokens, return true if possible, false otherwise. Blocking
  // call.
  bool Reserve(uint32_t tokens);

  // Reset all the counter.
  void Reset();

  // Reserve with callback. Useful when needs to wait until token is available.
  // When token is available callback method would be invoked.
  void Reserve(callback_function cf, uint32_t tokens,
               std::string custom_string = "");

private:
  RateLimitter(uint32_t rate, uint32_t burst);
  RateLimitter() {}
  float rate;
  float burst;
  std::chrono::time_point<std::chrono::system_clock> last_accessed_timestamp;
  float duration_to_token(float duration_in_secs);
  float token_to_duration_secs(float tokens);
  // how many tokens available
  float advance(std::chrono::time_point<std::chrono::system_clock> time_base);
  float available_tokens;
  std::mutex mtx;
};
