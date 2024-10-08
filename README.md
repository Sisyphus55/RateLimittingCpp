# Rate Limitting feature as Cpp library
Bazel project that implements thread safe, asynchronous token bucket based rate limiting algorithm. Appropriate test cases has been added to validate the functionality of Rate Limitter
# Usage

### Get rate limitter instance with rate and burst defined
```
RateLimitter *rt = RateLimitter::New(10, 20);
```
The above line would return a RateLimitter instance, with rate=10 and burst=20. The created instance can be used to access tokens.

### Using The Rate Limiter instance inside a application.
```
bool canAccess = rt->Reserve(1);
```
Quering where it is allowed to take 1 token that respects the rate and burst defined at the beginning. Similarly in order to request for n number of tokens, use **rt->Reserve(n)**. Check the **examples/** and **test/** directory for examples.

### Using The Rate Limiter as callback.
This is obvious that, **Reserve** API returns immediately whether token is allowed or not. Some application might require a callback, which would be invoked when the requested number of tokens are available. So this projects provides another API that exhibits this asynchronous behaviour. Below is a example to how use callback.
#### write your own callback
```
void callback(std::string str) {
  std::cout << "Token Is available!!" << std::endl;
  std::cout<<str<<std::endl;
}
```
#### Invoke Reserve API with the callback.
```
rt->Reserve(callback, 10, "10 tokens has been requested");
```
#### The whole code
```
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
```
The above code would call the callback after approximately 10 seconds and would print *10 tokens has been requested* . Note that, it is recommended to invoke **rt->Reserve(callback,10, "10 tokens has been requested")** inside a thread, so that the next line after can be executed without being blocked for 10 seconds.

### Reset to initial state.
```
rt->Reset();
```
This would reset the relevant counter to initial state. 

### Poll if token is accessible. 
```
bool token_available = rt->PollReserve(1);

if (token_available) std::cout<< "Token available, request can be made"<<std::endl;
else std::cout<<"Token not available"<<std::endl;
```
Sometimes client can just query if the tokens are available now. This is different from Reserve, as *Reserve* API **query and reserve** the token request. Whereas *PollReserve* API only **query** about the token request, no real token reservation is done".

