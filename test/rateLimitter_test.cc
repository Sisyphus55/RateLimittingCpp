#include "lib/rateLimitter.h"

#include <thread>

#include <gmock/gmock.h>

using testing::Eq;

// Use all the tokens at once.
TEST(RateLimitterTest, TokenGeneration) {
  int rate = 10;
  int burst = 10;
  RateLimitter *rt = RateLimitter::New(rate, burst);

  for (int i = 0; i < rate; i++) {
    bool can_access = rt->Reserve(1);
    EXPECT_TRUE(can_access);
  }
  EXPECT_FALSE(rt->Reserve(1));
}

// Use all the tokens in a consecutive sequence, wait for 1 seconds to generate
// all the tokens.
TEST(RateLimitterTest, ReTokenGeneration) {
  int rate = 10;
  int burst = 10;
  RateLimitter *rt = RateLimitter::New(rate, burst);

  for (int i = 0; i < rate; i++) {
    EXPECT_TRUE(rt->Reserve(1));
  }
  EXPECT_FALSE(rt->Reserve(1));
  std::this_thread::sleep_for(std::chrono::seconds(1));
  // 1 seconds delay to populate 10 tokens.
  for (int i = 0; i < rate; i++) {
    EXPECT_TRUE(rt->Reserve(1));
  }
}

// Test Reset functionality.
TEST(RateLimitterTest, ResetTokenGeneration) {
  int rate = 10;
  int burst = 10;
  RateLimitter *rt = RateLimitter::New(rate, burst);

  // Take all tokens at once.
  EXPECT_TRUE(rt->Reserve(rate));

  // Immmediately request for 1 tokens
  EXPECT_FALSE(rt->Reserve(1));

  // Reset token counter.
  rt->Reset();

  // Request for all tokens successful.
  EXPECT_TRUE(rt->Reserve(rate));
}

// Test Burst funtionality

TEST(RateLimitterTest, BurstTest) {
  int rate = 10;
  int burst = 20;
  RateLimitter *rt = RateLimitter::New(rate, burst);

  // wait for 4 seconds, then total token allowed should be 4 * rate(i.e.10)
  // = 40. Since burst is 20, no more than 20 tokens would be allowed

  std::this_thread::sleep_for(std::chrono::seconds(4));

  EXPECT_FALSE(rt->Reserve(rate * 3));
  EXPECT_TRUE(rt->Reserve(burst));
}

// Test callback

int global = -1;
void callback(std::string str) {
  std::cout << "Token Received" << std::endl;
  global = 1;
}

TEST(RateLimitterTest, CallBackTest) {
  int rate = 1;
  int burst = 1;
  RateLimitter *rt = RateLimitter::New(rate, burst);

  // Request for 10 tokens, so wait atleast 9 seconds to get the token. After 9
  // seconds, value of global would be updated inside callback.
  rt->Reserve(callback, 10, "Test");
  EXPECT_EQ(global, 1);
}
