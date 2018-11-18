#pragma once
#include <cmath>
#include <vector>
#include <exception>

template <typename T>
T norm(const std::vector<T>& a) {
  auto innerProduct = a * a;
  return sqrt(innerProduct);
}

template <typename T>
bool checkVector(const std::vector<T>& a, const std::vector<T>& b) {
  int aSize = a.size();
  int bSize = b.size();
  if(aSize != bSize){
    return false;
  }
  return true;
}

template <typename T>
T operator*(const std::vector<T>& a, const std::vector<T>& b) {
  T result = 0;
  if(!checkVector(a, b)){
    throw "operands must be same length";
  }
  for(size_t i = 0; i < a.size(); ++i){
    result += a[i]*b[i];
  }
  return result;
}

template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b) {
  std::vector<T> result;
  if(!checkVector(a, b)){
    throw "operands must be same length";
  }
  for(size_t i = 0; i < a.size(); i++) {
    result.emplace_back(a[i]-b[i]);
  }
  return result;
}
