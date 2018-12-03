#pragma once
#include <cmath>
#include <iostream>
#include "mfbase.hpp"
#include "../lib/vec.hpp"

#ifdef _OPENMP
#include <omp.h>
#endif

namespace MF {
  template<typename Type>
  class NMF : public MFBase<Type> {
    using TMatrix = std::vector<std::vector<Type>>;
    using MFBase<Type>::matrix;
    using MFBase<Type>::P;
    using MFBase<Type>::Q;
    using MFBase<Type>::k;
    using MFBase<Type>::thereshold;
    using MFBase<Type>::matrixRColNum;
    using MFBase<Type>::matrixRRowNum;

  public:
    NMF(std::unique_ptr<Matrix<Type>> m, double const thd, int const d) {
      matrix = std::move(m);
      thereshold = thd;
      k = d;
      matrixRRowNum = matrix->rowNum;
      matrixRColNum = matrix->colNum;
      P = std::make_unique<Matrix<Type>>(matrixRRowNum, k);
      Q = std::make_unique<Matrix<Type>>(matrixRColNum, k);
    }

    void execute(int const iteration, bool const verbose = false) {
      for(size_t itr = 0; itr < iteration; ++itr) {
        double rmse = 0;
        #pragma omp parallel for
        for(size_t u = 0; u < matrixRRowNum; ++u) {
          #pragma omp parallel for
          for(size_t i = 0; i < matrixRColNum; ++i) {
            std::vector<Type> p = P->getMatrixRow(u);
            std::vector<Type> q = Q->getMatrixRow(i);
            Type expectedr = p*q;
            Type error = expectedr - matrix->getMatrixElem(u, i);
            rmse += std::pow(error, 2);
            update(expected, u, i, p, q);
          }
        }

        rmse = std::sqrt(rmse / (matrix->colNum * matrix->rowNum));
        if (verbose) {
          std::cout << "=====================================================" << std::endl;
          std::cout << "Iteration: " << itr + 1 << std::endl;
          std::cout << "RMSE: " << rmse << std::endl;
        }
      }
    }

    void update(double const expected, 
                int const _u, int const _i, 
                std::vector<Type> p, std::vector<Type> q) {
      for(size_t k = 0; k < k; ++k) {
        double sum1 = 0;
        double sum2 = 0;
        double sum3 = 0;
        double sum4 = 0;

        for(size_t i = 0; i < matrix->colNum; ++i) {
          sum1 += matrix->getMatrixElem(_u, i) * Q->getMatrixElem(k, i);
          sum2 += expected * Q->getMatrixElem(k, i);
        }
        
        for(size_t u = 0; u < matrix->rowNum; ++u) {
          sum3 += matrix->getMatrixElem(u, _i) * Q->getMatrixElem(u, k);
          sum4 += expected * Q->getMatrixElem(u, k);
        }

        P->changeElem(_u, k, p[k]*(sum1/sum2));
        Q->changeElem(k, _i, q[k]*(sum3/sum4));
      }
    }
  };
};
