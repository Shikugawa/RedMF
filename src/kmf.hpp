#pragma once
#include <cmath>
#include <iostream>
#include <functional>
#include "mfbase.hpp"
#include "../lib/vec.hpp"

#ifdef _OPENMP
#include <omp.h>
#endif

namespace MF {
  template<typename Type>
  class KernelizedMF : public MFBase<Type> {
    using kFType = std::function<Type(std::vector<Type>, std::vector<Type>)>;
    using TMatrix = std::vector<std::vector<Type>>;
    using MFBase<Type>::matrix;
    using MFBase<Type>::P;
    using MFBase<Type>::Q;
    using MFBase<Type>::dim;
    using MFBase<Type>::thereshold;
    using MFBase<Type>::matrixRColNum;
    using MFBase<Type>::matrixRRowNum;

  public:
    KernelizedMF(std::unique_ptr<Matrix<Type>> m, double const thd, int const d) {
      matrix = std::move(m);
      thereshold = thd;
      dim = d;
      matrixRRowNum = matrix->rowNum;
      matrixRColNum = matrix->colNum;
      P = std::make_unique<Matrix<Type>>(matrixRRowNum, dim);
      Q = std::make_unique<Matrix<Type>>(matrixRColNum, dim);
    }

    void execute(kFType kernelFunction, bool const verbose = false) {
      #pragma omp parallel for
      for(size_t u = 0; u < matrixRRowNum; u++) {
        if (verbose) {
          std::cout << "Optimising user " << u << std::endl;
        }
        #pragma omp parallel for
        for(size_t i = 0; i < matrixRColNum; i++) {
          while(true) {
            std::vector<Type> p = P->getMatrixRow(u);
            std::vector<Type> q = Q->getMatrixRow(i);
            Type expectedr = kernelFunction(p, q);
            Type error = expectedr - matrix->getMatrixElem(u, i);
            if(std::pow(error, 2) < thereshold) break;
            update(0.001, error, u, i, p, q);
          }
        }
      }
    }

    void execute(kFType kernelFunction, int const iteration, bool const verbose = false) {
      for(size_t itr = 0; itr < iteration; ++itr) {
        double rmse = 0;
        #pragma omp parallel for
        for(size_t u = 0; u < matrixRRowNum; ++u) {
          #pragma omp parallel for
          for(size_t i = 0; i < matrixRColNum; ++i) {
            if (matrix->getMatrixElem(u, i) == 0)
              continue;
            std::vector<Type> p = P->getMatrixRow(u);
            std::vector<Type> q = Q->getMatrixRow(i);
            Type expectedr = kernelFunction(p, q);
            Type error = expectedr - matrix->getMatrixElem(u, i);
            rmse += std::pow(error, 2);
            update(0.0002, error, u, i, p, q);
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

    void update(double const alpha, Type const error, 
                int const u, int const i, 
                std::vector<Type> p, std::vector<Type> q) {
      for(size_t j = 0; j < dim; ++j) {
        auto& tmp = p[j];
        p[j] -= 2*alpha*error*q[j];
        q[j] -= 2*alpha*error*tmp;
        P->changeElem(u, j, p[j]);
        Q->changeElem(i, j, q[j]);
      }
    }
  };
};