#pragma once
#include <cmath>
#include <iostream>
#include <functional>
#include "mfbase.hpp"
#include "../lib/vec.hpp"
#include "../lib/kernel.hpp"
#ifdef _OPENMP
#include <omp.h>
#endif

namespace MF {
  template<typename Type, int KERNEL_NUM>
  class KernelizedMF : public MFBase<Type> {
    using kFType = std::function<Type(std::vector<Type>, std::vector<Type>)>;
    using TMatrix = std::vector<std::vector<Type>>;
    using MFBase<Type>::matrix;
    using MFBase<Type>::P;
    using MFBase<Type>::Q;
    using MFBase<Type>::k;
    using MFBase<Type>::thereshold;
    using MFBase<Type>::matrixRColNum;
    using MFBase<Type>::matrixRRowNum;

  public:  
    KernelizedMF(std::unique_ptr<Matrix<Type>> m, const int _k, const int _d,
                 const double _lambda, const double _learning_rate, std::array<kFType, KERNEL_NUM> _func):
                 d(_d), lambda(_lambda), learning_rate(_learning_rate) {
      k = _k;
      matrix = std::move(m);
      matrixRRowNum = matrix->rowNum;
      matrixRColNum = matrix->colNum;

      P = std::make_unique<Matrix<Type>>(matrixRRowNum, k);
      Q = std::make_unique<Matrix<Type>>(matrixRColNum, k);
      
      // == 補助変数 ==
      A = std::make_shared<Matrix<Type>>(k, matrixRRowNum);
      B = std::make_shared<Matrix<Type>>(k, matrixRColNum);
      // == 補助変数 ==

      D = std::make_shared<Matrix<Type>>(d, k); // 論文中における「Dictionary vector)
      multi_kernel = std::make_unique<MultiKernel<Type, KERNEL_NUM>>(D, _func, k); // マルチカーネルの初期化 
    }

    void execute(const int iteration) {
      for(std::size_t itr = 0; itr < iteration; ++itr) {
        std::cout << "=====================================================" << std::endl;
        std::cout << "Iteration: " << itr + 1 << std::endl;

        double sum_error = 0;
        std::uint32_t rated = 0;

        #pragma omp parallel for
        for(std::size_t u = 0; u < matrixRRowNum; ++u) {
          std::cout << u << std::endl;
          #pragma omp parallel for
          for(std::size_t i = 0; i < matrixRColNum; ++i) {
            std::vector<Type> a_u = A->getMatrixCol(u);
            std::vector<Type> b_i = B->getMatrixCol(i);
            Type real_value = matrix->getMatrixElem(u, i);
            
            if(real_value != 0){
              auto _single_kernels = multi_kernel->single_kernels;
              auto _kernel_weights = multi_kernel->kernel_weights;
              TMatrix _K;

              for(std::size_t i = 0; i < _kernel_weights.size(); ++i) {
                if(i == 0) {
                  _K = _kernel_weights[i]*_single_kernels[i]->K->getMatrix();
                }
                _K = _K + _kernel_weights[i]*_single_kernels[i]->K->getMatrix();
              }

              Type expected_value = a_u*_K*b_i;
              Type error = real_value - expected_value;
              sum_error += std::pow(error, 2);
              update(a_u, b_i, new Matrix<Type>(_K), u, i, error);

              // == update kernel weight ==
              multi_kernel->calcY(matrixRRowNum, matrixRColNum, A, B);
              multi_kernel->calcZ(matrixRRowNum, matrixRColNum, lambda, real_value, A, B);
              // Solve Quadratic Problem
              
              // == update kernel weight ==

              rated++;
            }
          }
        }

        double rmse = std::sqrt(sum_error/rated);
        std::cout << "RMSE: " << rmse << std::endl;
        std::cout << "=====================================================" << std::endl;
      }
    }

    void update(const std::vector<Type>& _a_u, const std::vector<Type>& _b_i, Matrix<Type>*&& K,
                const int _u, const int _i, const Type error){
      for(size_t j = 0; j < k; ++j) {
        A->changeElem(
          j, _u, 
          A->getMatrixElem(j, _u)+learning_rate*2*error*K->getMatrixCol(j)*_b_i
        );
        B->changeElem(
          j, _i,
          B->getMatrixElem(j, _i)+learning_rate*2*error*K->getMatrixRow(j)*_a_u
        );
      }

      delete K;
    }

  private:
    std::uint8_t d;
    double lambda, learning_rate;
    std::shared_ptr<Matrix<Type>> A, B, D;
    std::unique_ptr<MultiKernel<Type, KERNEL_NUM>> multi_kernel;
  };
};