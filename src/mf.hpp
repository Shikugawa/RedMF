 // Reference: http://www.quuxlabs.com/blog/2010/09/matrix-factorization-a-simple-tutorial-and-implementation-in-python/

#include <vector>
#include <array>
#include <memory>
#include <cmath>
#include <iostream>
#include "../lib/matrix.hpp"
#include "../lib/vec.hpp"

template<typename Type> class MatrixFactorization {
  typedef std::vector<std::vector<Type>> TMatrix;

public:
  MatrixFactorization(std::unique_ptr<Matrix<Type>> m, double const thereshold, int const dim) {
    matrix = std::move(m);
    this->thereshold = thereshold;
    this->dim = dim;
    matrixRRowNum = matrix->rowNum;
    matrixRColNum = matrix->colNum;
    P = std::make_unique<Matrix<Type>>(matrixRRowNum, dim);
    Q = std::make_unique<Matrix<Type>>(matrixRColNum, dim);
  }

  TMatrix getPMatrix() {
    return P->getMatrix();
  }

  TMatrix getQMatrix() {
    return Q->getMatrix();
  }

  void execute (bool const verbose = false) {
    for(size_t u = 0; u < matrixRRowNum; u++) {
      if (verbose) {
        std::cout << "Optimising user " << u << std::endl;
      }

      for(size_t i = 0; i < matrixRColNum; i++) {
        while(true) {
          std::vector<Type> p = P->getMatrixRow(u);
          std::vector<Type> q = Q->getMatrixRow(i);
          Type expectedr = mul<Type>(p, q);
          Type error = expectedr - matrix->getMatrixElem(u, i);
          if(std::pow(error, 2) < thereshold) break;
          update(0.01, error, u, i, p, q);
        }
      }
    }
  }

private:
  std::unique_ptr<Matrix<Type>> matrix, P, Q;
  double thereshold;
  int dim, matrixRRowNum, matrixRColNum;

  TMatrix getFilledMatrix(Type const value, int const rowNum, int const colNum) {
    TMatrix filled;
    for(size_t i = 0; i < rowNum; ++i) {
      filled.push_back({});     
      for(size_t j = 0; j < colNum; ++j)
        filled[i].push_back(value);
    }

    return filled;
  };

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
