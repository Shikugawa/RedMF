#pragma once
#include <vector>
#include <cmath>
#include "vec.hpp"

template <typename T>
class KernelFunctions {
public:
  static T gaussianKernel(std::vector<T> a, std::vector<T> b) {
    double sigma = 30;
    T squaredNorm = pow(norm(a-b), 2);
    return -exp(squaredNorm / pow(sigma, 2));
  }
};