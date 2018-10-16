#include "../lib/matrix.hpp"
#include <vector>
#include <memory>

template<typename Type> 
class MatrixFactorizationBase {
  typedef std::vector<std::vector<Type>> TMatrix;

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

  virtual void execute (bool const verbose = false) = 0;

  TMatrix getFilledMatrix(Type const value, int const rowNum, int const colNum) {
    TMatrix filled;
    for(size_t i = 0; i < rowNum; ++i) {
      filled.push_back({});     
      for(size_t j = 0; j < colNum; ++j)
        filled[i].push_back(value);
    }

    return filled;
  };

  virtual void update(double const alpha, Type const error, 
              int const u, int const i, 
              std::vector<Type> p, std::vector<Type> q) = 0;
};

