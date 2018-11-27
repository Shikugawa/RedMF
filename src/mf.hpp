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
  class MatrixFactorization : public MFBase<Type> {
    using TMatrix = std::vector<std::vector<Type>>;
    using MFBase<Type>::matrix;
    using MFBase<Type>::P;
    using MFBase<Type>::Q;
    using MFBase<Type>::dim;
    using MFBase<Type>::thereshold;
    using MFBase<Type>::matrixRColNum;
    using MFBase<Type>::matrixRRowNum;

  public:
    MatrixFactorization(std::unique_ptr<Matrix<Type>> m, double const thd, int const d) {
      matrix = std::move(m);
      thereshold = thd;
      dim = d;
      matrixRRowNum = matrix->rowNum;
      matrixRColNum = matrix->colNum;
      P = std::make_unique<Matrix<Type>>(matrixRRowNum, dim);
      Q = std::make_unique<Matrix<Type>>(matrixRColNum, dim);
    }

    void execute(bool const verbose = false) {
      #pragma omp parallel for
      for(size_t u = 0; u < matrixRRowNum; u++) {
        if (verbose) {
          std::cout << "Optimising user " << u << std::endl;
        }
        #pragma omp parallel for
        for(size_t i = 0; i < matrixRColNum; i++) {
          if(matrix->getMatrixElem(u, i) == 0)
              continue;

          while(true) {
            std::vector<Type> p = P->getMatrixRow(u);
            std::vector<Type> q = Q->getMatrixRow(i);
            Type expectedr = p*q;
            Type error = expectedr - matrix->getMatrixElem(u, i);
            if(std::pow(error, 2) < thereshold) break;
            update(0.001, error, u, i, p, q);
          }
        }
      }
    }

    void execute(int const iteration, bool const verbose = false) {
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
              Type error_squared = std::pow(p*q-real_value, 2);
              Type error = std::sqrt(error_squared);
              if(error == NAN || error == INFINITY) {
                exit(EXIT_FAILURE);
              }
              if (error > 10){
                for(auto _p: p){
                  std::cout << _p << " ";
                }
                std::cout << "\n";
                for(auto _p: q){
                  std::cout << _p << " ";
                }
                std::cout << "\n";
                std::cout << p*q << " " << real_value << " " << error << std::endl;
              }
                
              sum_error += error_squared;
              update(0.0002, error, u, i);
              rated++;
            }
          }
        }
        std::cout << sum_error << " " << rated << std::endl;
        std::cout << "RMSE: " << std::sqrt(sum_error/rated) << std::endl;
        std::cout << "=====================================================" << std::endl;
      }
    }

    void update(double const alpha, Type const error, 
                int const u, int const i) {
      for(size_t j = 0; j < dim; ++j) {
        // auto tmp = p[j];
        // std::cout << p[j] << std::endl;
        // if(p[j]+alpha*(2*error*q[j]-0.02*p[j]) > 10){
        //   // std::cout << p[j] << " " << q[j] << std::endl;
        //   std::cout << p[j]+alpha*(2*error*q[j]-0.02*p[j]) << std::endl;
        // }
        auto _p = P->getMatrixElem(u, j);
        auto _q = Q->getMatrixElem(i, j);
        // std::cout << _p+alpha*(2*error*_q-0.02*_p) << std::endl;
        P->changeElem(u, j, _p+alpha*(2*error*_q-0.02*_p));
        Q->changeElem(i, j, _q+alpha*(2*error*_p-0.02*_q));
      }
    }
  };
};
