#pragma once
#include <cmath>
#include <vector>
#include <exception>
#include <random>

std::vector<double> initRandomVector(std::size_t d) {
  std::vector<double> result;
  std::random_device rnd;
  std::mt19937 mt(rnd());
  std::uniform_int_distribution<> rand(0, 1);
  for(std::size_t i = 0; i < d; ++i) {
    result.emplace_back(rand(mt));
  }
  return result;
}

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
std::vector<T> operator*(const T a, const std::vector<T>& b) {
  std::vector<T> result;
  for(size_t i = 0; i < b.size(); ++i) {
    result.emplace_back(a*b[i]);
  }
  return result;
}

template <typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
  std::vector<T> result;
  for(size_t i = 0; i < a.size(); ++i) {
    result.emplace_back(a[i]+b[i]);
  }
  return result;
}

template <typename T>
std::vector<T> operator*(const std::vector<T>& a, const std::vector<std::vector<T>>& b) {
  std::vector<T> result;
  for(size_t i = 0; i < b[0].size(); ++i){
    T r = 0;
    for(size_t j = 0; j < b.size(); ++j){
      r += a[j]*b[j][i];
    }
    result.emplace_back(r);
  }
  return result;
}

template <typename T>
std::vector<T> operator*(const std::vector<std::vector<T>>& a, const std::vector<T>& b) {
  std::vector<T> result;
  for(size_t i = 0; i < a.size(); ++i){
    T r = 0;
    for(size_t j = 0; j < a[0].size(); ++j){
      r += a[i][j]*b[j];
    }
    result.emplace_back(r);
  }
  return result;
}

template <typename T>
std::vector<std::vector<T>> operator*(const T a, std::vector<std::vector<T>>& b) {
  std::vector<std::vector<T>> result;
  for(size_t i = 0; i < b.size(); ++i){
    std::vector<T> r;
    for(size_t j = 0; j < b[0].size(); ++j){
      r.emplace_back(a*b[i][j]);
    }
    result.emplace_back(r);
  }
  return result;
}

template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const std::vector<T>& b) {
  std::vector<T> result;
  if(!checkVector(a, b)){
    throw "operands must be same length";
  }
  for(size_t i = 0; i < a.size(); ++i) {
    result.emplace_back(a[i]-b[i]);
  }
  return result;
}
