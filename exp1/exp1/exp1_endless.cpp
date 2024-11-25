#include <stdio.h>

long long int fibonacci(int n){
  if(n == 0 || n == 1){
    return 1;
  }
  return fibonacci(n - 1) + fibonacci(n - 2);
}
int main()
{
  for(int i = 0; i < 64; i++){
    printf("%lld %d\n", fibonacci(i + 1), i);
  }
  return 0;
}
