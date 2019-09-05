#pragma once
#include <vector>
#include <cmath>

namespace RedMF::Kernel {
  template <typename T>
  class KernelFunctions {
  public:
    static T gaussianKernel(const std::vector<T>& a, const std::vector<T>& b) {
      T squaredNorm = pow(norm(a-b), 2);
      return std::exp(-(squaredNorm / std::pow(0.1, 2)));
    }
  };
}
