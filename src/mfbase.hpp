#pragma once
#include "../lib/matrix.hpp"
#include "../lib/vec.hpp"
#include <cmath>
#include <vector>
#include <memory>
#include <functional>

template<typename Type>
class MFBase {
  using TMatrix = std::vector<std::vector<Type>>;
  using kFType = std::function<Type(std::vector<Type>, std::vector<Type>)>;

public:
  std::unique_ptr<Matrix<Type>> matrix, P, Q;
  double thereshold;
  int k, matrixRRowNum, matrixRColNum;
  
  TMatrix getPMatrix() {
    return P->getMatrix();
  }

  TMatrix getQMatrix() {
    return Q->getMatrix();
  }

  TMatrix getFilledMatrix(Type const value, int const rowNum, int const colNum) {
    TMatrix filled;
    for(size_t i = 0; i < rowNum; ++i) {
      filled.emplace_back({});     
      for(size_t j = 0; j < colNum; ++j)
        filled[i].emplace_back(value);
    }

    return filled;
  };

  virtual Type RMSE(size_t matrix_col, size_t matrix_row, Matrix<Type>& iuMatrix, Matrix<Type>& P, Matrix<Type>& Q) {
    double result = 0;
    for(size_t u = 0; u < matrix_row; u++) {
      for(size_t i = 0; i < matrix_col; i++) {
        std::vector<Type> pu = P.getMatrixRow(u);
        std::vector<Type> qi = Q.getMatrixRow(i);
        result += std::pow(iuMatrix.getMatrixElem(u, i)-pu*qi, 2);
      }
    }
    return std::sqrt(result/(matrix_col*matrix_row));
  }
};
