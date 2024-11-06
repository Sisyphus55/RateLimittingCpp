
#include "stdio.h"
#include "lib/rateLimitter.h"

void callback(std::string str) {
  std::cout << "Token Is available!!" << std::endl;
  std::cout<<str<<std::endl;
}

int main() {
  RateLimitter *rt = RateLimitter::New(1, 1); //would take 10 seconds to generate 10 tokens.
  rt->Reserve(callback,10, "10 tokens has been requested");
  return 0;
}
