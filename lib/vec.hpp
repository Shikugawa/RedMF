#include <vector>
#include <exception>

template <typename T>
std::vector<T> mul(const std::vector<T> a, const std::vector<T> b) {
  int aSize = a.size();
  int bSize = b.size();

  if(aSize != bSize){
    throw "operands must be same length";
  } 

  std::vector<T> result;
  for(size_t i = 0; i < aSize; ++i){
    T ans;
    for(size_t j = 0; j < bSize; ++j)
      ans += a[i]*b[j];
    result.push_back(ans);
  }
  return result;
}
