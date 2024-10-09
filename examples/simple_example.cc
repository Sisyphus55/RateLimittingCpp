
#include "lib/rateLimitter.h"
#include "stdio.h"
int main() {
  RateLimitter *rt = RateLimitter::New(10, 10);//Inititialize rateLimitter with rate =10 and burst = 10
  for (int i = 0; i <= 10; i++) {
    bool canAccess = rt->Reserve(1);
    if (canAccess) {
      std::cout << "Token available" << std::endl;
    } else
      std::cout << "Token not available" << std::endl;
  }

  return 0;
}
