
#include "rateLimitter.h"

RateLimitter::RateLimitter(uint32_t rate, uint32_t burst) {
  this->rate = static_cast<float>(rate);
  this->burst = static_cast<float>(burst);
  this->last_accessed_timestamp = std::chrono::system_clock::now();
  this->available_tokens = rate;
}

RateLimitter *RateLimitter::New(uint32_t rate, uint32_t burst) {
  RateLimitter *rt = new RateLimitter(rate, burst);

  return rt;
}

void RateLimitter::Reserve(callback_function cf, uint32_t tokens,
                           std::string str) {

  float total_wait_time_in_secs = 0.0;
  {
    std::unique_lock<std::mutex> lck(mtx);
    std::chrono::time_point<std::chrono::system_clock> time_now =
        std::chrono::system_clock::now();
    float token_available = advance(time_now);

    if (token_available < tokens)
      total_wait_time_in_secs =
          token_to_duration_secs(tokens - token_available);
  }
  std::chrono::duration<float> sleep_duration(total_wait_time_in_secs);
  std::this_thread::sleep_for(sleep_duration);
  cf(str);
}

bool RateLimitter::Reserve(uint32_t tokens) {

  std::unique_lock<std::mutex> lck(mtx);

  std::chrono::time_point<std::chrono::system_clock> time_now =
      std::chrono::system_clock::now();

  float token_available = advance(time_now);

  if (token_available < tokens)
    return false;

  available_tokens = token_available - tokens;

  last_accessed_timestamp = time_now;

  return true;
}

bool RateLimitter::PollReserve(uint32_t tokens) {
	std::chrono::time_point<std::chrono::system_clock> time_now =
      std::chrono::system_clock::now();

  	float token_available = advance(time_now);

	if (token_available >= tokens) return true;

	else return false;
}


float RateLimitter::advance(
    std::chrono::time_point<std::chrono::system_clock> time_base) {
  int duration_in_useconds =
      std::chrono::duration_cast<std::chrono::microseconds>(
          time_base - last_accessed_timestamp)
          .count();

  float tokens = duration_to_token(static_cast<float>(duration_in_useconds));

  tokens += available_tokens;

  if (tokens > burst)
    tokens = burst;

  return tokens;
}

float RateLimitter::duration_to_token(float duration_in_usecs) {
  return (duration_in_usecs * rate) / 1e6;
}

float RateLimitter::token_to_duration_secs(float tokens) {
  float duration = tokens / rate;
  return duration;
}

void RateLimitter::Reset() {
  std::unique_lock<std::mutex> lck(mtx);
  this->last_accessed_timestamp = std::chrono::system_clock::now();
  this->available_tokens = rate;
}
