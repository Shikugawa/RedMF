#include <vector>
#include <exception>

template <typename T>
T mul(const std::vector<T> a, const std::vector<T> b) {
  int aSize = a.size();
  int bSize = b.size();

  if(aSize != bSize){
    std::cout << aSize << std::endl;
    std::cout << bSize << std::endl;
    throw "operands must be same length";
  } 

  T result = 0;
  for(size_t i = 0; i < aSize; ++i){
    result += a[i]*b[i];
  }
  return result;
}
