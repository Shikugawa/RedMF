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
  MatrixFactorization(Matrix<Type>* m, double const thereshold, int const dim) {
    this->matrix = m;
    this->thereshold = thereshold;
    this->dim = dim;
    this->matrixRRowNum = this->matrix->getRowNum();
    this->matrixRColNum = this->matrix->getColNum();
    P = std::make_unique<Matrix<Type>>(this->getFilledMatrix(1.0, this->matrixRRowNum, this->dim));
    Q = std::make_unique<Matrix<Type>>(this->getFilledMatrix(1.0, this->matrixRColNum, this->dim));
  }

  ~MatrixFactorization() { delete matrix; }

  TMatrix getPMatrix() {
    return this->P;
  }

  TMatrix getQMatrix() {
    return this->Q;
  }

  void execute (bool const verbose = false) {
    for(size_t u = 0; u < this->matrixRRowNum; u++) {
      for(size_t i = 0; i < this->matrixRColNum; i++) {
        if (verbose) {
          std::cout << "Optimising user " << u << " item " << i << std::endl;
        }

        while(true) {
          std::vector<Type> p = this->P->getMatrixRow(u);
          std::vector<Type> q = this->Q->getMatrixRow(i);
          Type expectedr = mul<Type>(p, q);
          Type error = expectedr - this->matrix->getMatrixElem(u, i);
          if(std::pow(error, 2) < this->thereshold) break;
          this->update(0.0002, error, u, i, p, q);
        }
      }
    }
  }

private:
  Matrix<Type>* matrix;
  std::unique_ptr<Matrix<Type>> P, Q;
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
    for(size_t j = 0; j < this->dim; ++j) {
      auto& tmp = p[j];
      p[j] -= 2*alpha*error*q[j];
      q[j] -= 2*alpha*error*tmp;
      this->P->changeElem(u, j, p[j]);
      this->Q->changeElem(i, j, q[j]);
    }
  }
};
