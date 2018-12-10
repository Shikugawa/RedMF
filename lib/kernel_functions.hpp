#pragma once
#include <vector>
#include <cmath>

template <typename T>
class KernelFunctions {
public:
  static T gaussianKernel(std::vector<T> a, std::vector<T> b) {
    T squaredNorm = pow(norm(a-b), 2);
    return std::exp(-(squaredNorm / std::pow(0.1, 2)));
  }

  static T linearKernel(std::vector<T> a, std::vector<T> b) {

  }
};