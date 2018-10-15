#include <eigen3/Eigen/Core>
#include <vector>
#include <memory>
#include <exception>

template <typename T> class Matrix {
  typedef std::vector<std::vector<T>> TMatrix;

public:
  std::shared_ptr<Eigen::MatrixXd> matrix;
  int colNum, rowNum;

  Matrix(TMatrix R) {
    rowNum = R.size();
    colNum = R[0].size();
    tMatrixToEigenMatrix(R);
  }

  inline TMatrix getMatrix() {
    return eigenMatrixToSTLVector(colNum, rowNum);
  };

  inline T getMatrixElem(int x, int y) const {
    auto tmp = matrix.get();
    return matrix->coeff(x, y);
  };

  inline void changeElem(int x, int y, T value) {
    auto& tmp = *matrix;
    tmp(x, y) = value;
  }

  std::vector<T> getMatrixRow(int x) {
    if(x >= rowNum) throw "invalid argument";
    return eigenVectorToSTLVector((Eigen::VectorXd)matrix->row(x), colNum);
  }

  std::vector<T> getMatrixCol(int y) {
    if(y >= colNum) throw "invalid argument";
    return eigenVectorToSTLVector((Eigen::VectorXd)matrix->col(y), rowNum);
  };

  TMatrix getTranspose() {
    TMatrix pTransposed;
    for(size_t i = 0; i < colNum; i++) {
      std::vector<T> v;
      for(size_t j = 0; j < rowNum; j++) {
        v.push_back(matrix->coeff(j, i));
      }
      pTransposed.push_back(v);
    }
    return pTransposed;
  };
  
private:
  TMatrix getFilledMatrix(int num) {
    TMatrix filled;
    for(size_t i = 0; i < rowNum; ++i) {
      filled.push_back({});     
      for(size_t j = 0; j < colNum; ++j)
        filled[i].push_back(0);
    }

    return filled;
  };

  TMatrix eigenMatrixToSTLVector(int col, int row) {
    TMatrix result;
    for(size_t i = 0; i < row; i++) {
      std::vector<T> v;
      for(size_t j = 0; j < col; j++) {
        v.push_back(matrix.get()->coeff(i, j));
      }
      result.push_back(v);
    }
    return result;
  }

  std::vector<T> eigenVectorToSTLVector(Eigen::VectorXd vector, int size) {
    std::vector<T> result;
    for(size_t i = 0; i < size; i++) {
      result.push_back(vector.coeff(i));
    }

    return result;
  }

  void tMatrixToEigenMatrix(TMatrix m) {
    std::unique_ptr<Eigen::MatrixXd> tmpMatrix = std::make_unique<Eigen::MatrixXd>(rowNum, colNum);
    for(size_t i = 0; i < rowNum; i++) {
      for(size_t j = 0; j < colNum; j++) {
        tmpMatrix->coeffRef(i, j) = m[i][j];
      }
    }
    matrix = std::move(tmpMatrix);
  }
};
