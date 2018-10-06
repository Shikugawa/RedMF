#include <vector>
#include <array>
#include <memory>
#include "lib/matrix.hpp"
#include "lib/vec.hpp"

template<typename Type> class MatrixFactrization {
  typedef std::vector<std::vector<Type>> TMatrix;

public:
  MatrixFactrization(const Matrix<Type>* m, double const thereshold, int const dim) {
    this->matrix = m;
    this->thereshold = thereshold;
    this->dim = dim;
    this->matrixRRowNum = this->matrix->getRowNum();
    this->matrixRColNum = this->matrix->getColNum();
    P = std::make_unique<Matrix<Type>>(this->getFilledMatrix(1.0, this->dim, this->matrixRRowNum));
    Q = std::make_unique<Matrix<Type>>(this->getFilledMatrix(1.0, this->dim, this->matrixRColNum));
  }

  std::unique_ptr<std::array<TMatrix>> execute () {
    for(size_t u = 0; u < this->matrixRRowNum; u++) {
      for(size_t i = 0; i < this->matrixRColNum; i++) {
        std::vector<Type> p = this->P->getMatrixRow(u);
        std::vector<Type> q = this->P->getMatrixCol(i);
        std::vector<Type> expectedr = mul<Type>(p, q);
        Type error = pow(expectedr - this->matrix->getMatrixElem(u, i), 2);
        if(error < this->thereshold)
          return std::make_unique<std::array>TMatrix>>(this->P, this->Q);

        this->update();
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
        filled[i].push_back(0);
    }

    return filled;
  };

  void update(double const alpha) {

  }
};
