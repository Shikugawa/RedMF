#pragma once
#include "../lib/matrix.hpp"
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
  int dim, matrixRRowNum, matrixRColNum;
  
  TMatrix getPMatrix() {
    return P->getMatrix();
  }

  TMatrix getQMatrix() {
    return Q->getMatrix();
  }

  // virtual void execute (bool const verbose = false) = 0;
  // virtual void execute (int const iteration, bool const berbose = false) = 0;
  // virtual void execute (kFType kernelFunction, int const iteration, bool const berbose = false) = 0;

  virtual void update(double const alpha, Type const error, 
              int const u, int const i, 
              std::vector<Type> p, std::vector<Type> q) = 0;

  TMatrix getFilledMatrix(Type const value, int const rowNum, int const colNum) {
    TMatrix filled;
    for(size_t i = 0; i < rowNum; ++i) {
      filled.emplace_back({});     
      for(size_t j = 0; j < colNum; ++j)
        filled[i].emplace_back(value);
    }

    return filled;
  };
};

