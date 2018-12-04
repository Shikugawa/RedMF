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
    using MFBase<Type>::k;
    using MFBase<Type>::thereshold;
    using MFBase<Type>::matrixRColNum;
    using MFBase<Type>::matrixRRowNum;

  public:
    KernelizedMF(std::unique_ptr<Matrix<Type>> m, const int _k, const int _d,
                 const double _lambda, const double _learning_rate, kFType _kF) {
      matrix = std::move(m);
      k = _k;
      d = _d;
      lambda = _lambda;
      learning_rate = _learning_rate;
      matrixRRowNum = matrix->rowNum;
      matrixRColNum = matrix->colNum;
      kernelFunction = _kF;

      P = std::make_unique<Matrix<Type>>(matrixRRowNum, k);
      Q = std::make_unique<Matrix<Type>>(matrixRColNum, k);
      
      // == 補助変数 ==
      A = std::make_unique<Matrix<Type>>(k, matrixRRowNum);
      B = std::make_unique<Matrix<Type>>(k, matrixRColNum);
      // == 補助変数 ==

      D = std::make_unique<Matrix<Type>>(d, k); // 論文中における「Dictionary vector」
      K = std::make_unique<Matrix<Type>>(k, k); // TODO: Citrus::Linear側で初期化時に実行する関数を渡す機能を実装する

      initializeGramMatrix(); // グラム行列の初期化
    }

    void execute(int const iteration, const bool verbose = true) {
      for(std::size_t itr = 0; itr < iteration; ++itr) {
        double rmse = 0;
        #pragma omp parallel for
        for(std::size_t u = 0; u < matrixRRowNum; ++u) {
          #pragma omp parallel for
          for(std::size_t i = 0; i < matrixRColNum; ++i) {
            if(matrix->getMatrixElem(u, i) == 0)
              continue;

            std::vector<Type> a_u = A->getMatrixCol(u);
            std::vector<Type> b_i = B->getMatrixCol(i);
            Type expected_value = a_u*K->getMatrix()*b_i;
            
            Type error = matrix->getMatrixElem(u, i) - expected_value;
            rmse += error;  
            update(a_u, b_i, u, i, error);
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

    void update(const std::vector<Type>& _a_u, const std::vector<Type>& _b_i, const int _u, const int _i, const Type error){
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
    }

  private:
    std::uint8_t d;
    double lambda, learning_rate;
    std::unique_ptr<Matrix<Type>> A, B, D, K;
    kFType kernelFunction;

    void initializeGramMatrix() {
      for(std::size_t i = 0; i < k; ++i) {
        for(std::size_t j = 0; j < k; ++j) {
          K->changeElem(i, j, kernelFunction(
            D->getMatrixCol(i),
            D->getMatrixCol(j)
          ));
        }
      }
    }
  };
};