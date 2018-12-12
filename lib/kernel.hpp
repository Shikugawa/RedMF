#pragma once
#include <vector>
#include <functional>
#include <cmath>
#include <array>
#include "vec.hpp"
#include "matrix.hpp"
#include "single_Kernel.hpp"

#ifdef _OPENMP
#include <omp.h>
#endif

template <typename T>
using kernelFuncType = std::function<T(std::vector<T>, std::vector<T>)>;

template <typename T, int KERNEL_NUM>
class MultiKernel {
  using TMatrix = std::vector<std::vector<T>>;

public:
  std::size_t k, p;
  std::shared_ptr<Matrix<T>> D;
  std::array<std::shared_ptr<SingleKernel<T>>, KERNEL_NUM> single_kernels;
  std::array<double, KERNEL_NUM> kernel_weights;
  
  // FIXME: use opearator* from Citrus::Linear::Vector class
  std::function<TMatrix(std::vector<T>, std::vector<T>)> calc = [](std::vector<T> a, std::vector<T> b){
    TMatrix result;
    for(std::size_t i = 0; i < a.size(); ++i) {
      std::vector<T> tmp;
      for(std::size_t j = 0; j < b.size(); ++j) {
        tmp.emplace_back(a[i]*b[j]);
      }
      result.emplace_back(tmp);
    }
    return result;
  };

  MultiKernel(std::shared_ptr<Matrix<T>>& _D, std::array<kernelFuncType<T>, KERNEL_NUM> _kernel_functions, const int _k):
              k(_k), D(_D) {
    p = _kernel_functions.size();
    for(size_t i = 0; i < p; ++i) {
      single_kernels[i] = std::make_shared<SingleKernel<T>>(_kernel_functions[i], D, k);
      kernel_weights[i] = 1.0/p;
    }

    Y = std::make_unique<Matrix<T>>(p, p);
    Z = initRandomVector(p);
  }

  void calcY(std::size_t user_num, std::size_t item_num, std::shared_ptr<Matrix<T>>& _A, std::shared_ptr<Matrix<T>>& _B) {
    TMatrix y;

    // #pragma omp parallel for
    for(std::size_t user = 0; user < user_num; ++user) {
      for(std::size_t item = 0; item < item_num; ++item) {
        std::vector<T> v_ui;
        for(std::size_t i = 0; i < p; ++i) {
          v_ui.emplace_back(_A->getMatrixCol(user)*single_kernels[i]->K->getMatrix()*_B->getMatrixCol(item));
        }
        y = y + calc(v_ui, v_ui);
        std::cout << user << " " << item << std::endl;
      }
    }

    for(std::size_t i = 0; i < y.size(); ++i) {
      for(std::size_t j = 0; j < y[0].size(); ++j) {
        Y->changeElem(i, j, y[i][j]);
      }
    }
  }

  void calcZ(std::size_t user_num, std::size_t item_num, double _lambda, double real_value,
             std::shared_ptr<Matrix<T>>& _A, std::shared_ptr<Matrix<T>>& _B) {
    std::vector<T> _z;
    for(std::size_t user = 0; user < user_num; ++user) {
      for(std::size_t item = 0; item < item_num; ++item) {
        std::vector<T> gamma_ui;
        for(std::size_t i = 0; i < p; ++i) {
          auto left = _A->getMatrixCol(user)*single_kernels[i]->K->getMatrix()*_A->getMatrixCol(user);
          auto right = _B->getMatrixRow(item)*single_kernels[i]->K->getMatrix()*_B->getMatrixRow(item);
          auto value = left + right;
          gamma_ui.emplace_back(value);
        }

        _z = _z + (_lambda - 2*real_value)*gamma_ui;
      }
    }

    Z = _z;
  }

private:
  std::unique_ptr<Matrix<T>> Y;
  std::vector<T> Z;
};
