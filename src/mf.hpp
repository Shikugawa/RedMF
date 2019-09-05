#pragma once
#include <cmath>
#include <iostream>
#include "mfbase.hpp"
#include "../lib/linear/vec.hpp"

#ifdef _OPENMP
#include <omp.h>
#endif

namespace RedMF::Factorization {
  template<typename Type>
  class MatrixFactorization : public MFBase<Type> {
    using TMatrix = std::vector<std::vector<Type>>;
    using MFBase<Type>::matrix;
    using MFBase<Type>::P;
    using MFBase<Type>::Q;
    using MFBase<Type>::k;
    using MFBase<Type>::thereshold;
    using MFBase<Type>::matrixRColNum;
    using MFBase<Type>::matrixRRowNum;

  public:
    MatrixFactorization(std::unique_ptr<Matrix<Type>> m, int const d) {
      matrix = std::move(m);
      k = d;
      matrixRRowNum = matrix->rowNum;
      matrixRColNum = matrix->colNum;
      P = std::make_unique<Matrix<Type>>(matrixRRowNum, k);
      Q = std::make_unique<Matrix<Type>>(matrixRColNum, k);
    }

    std::vector<Type> execute(int const iteration, double thereshold = 0.5) {
      std::vector<Type> iteration_rmse({});
      for(size_t itr = 0; itr < iteration; ++itr) {
        std::cout << "=====================================================" << std::endl;
        std::cout << "Iteration: " << itr + 1 << std::endl;
        
        double sum_error = 0;
        std::uint32_t rated = 0;

        #pragma omp parallel for
        for(size_t u = 0; u < matrixRRowNum; ++u) {
          #pragma omp parallel for
          for(size_t i = 0; i < matrixRColNum; ++i) {
            std::vector<Type> p = P->getMatrixRow(u);
            std::vector<Type> q = Q->getMatrixRow(i);
            Type real_value = matrix->getMatrixElem(u, i);

            if(real_value != 0){
              Type error = real_value-p*q;
              sum_error += std::pow(error, 2);
              update(0.0002, error, u, i);
              rated++;
            }
          }
        }

        double rmse = std::sqrt(sum_error/rated);
        iteration_rmse.emplace_back(rmse);
        std::cout << "RMSE: " << rmse << std::endl;
        std::cout << "=====================================================" << std::endl;
      }
    }

    void update(const double alpha, Type error, 
                const int u, const int i, const double lambda = 0.02) {
      for(size_t j = 0; j < k; ++j) {
        P->changeElem(
          u, j, 
          P->getMatrixElem(u, j)+alpha*(2*error*Q->getMatrixElem(i, j)-lambda*P->getMatrixElem(u, j))
        );
        Q->changeElem(
          i, j,
          Q->getMatrixElem(i, j)+alpha*(2*error*P->getMatrixElem(u, j)-lambda*Q->getMatrixElem(i, j))
        );
      }
    }
  };
};
