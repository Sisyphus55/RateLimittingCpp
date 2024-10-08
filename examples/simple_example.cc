
#include "stdio.h"
#include  "lib/rateLimitter.h"
int main () {
	RateLimitter* rt = RateLimitter::New(10,10);
  	for (int i=0;i<=11;i++) std::cout<<rt->Reserve(1)<<std::endl;
  	std::cout<<"hello world\n";

	return 0;
}
