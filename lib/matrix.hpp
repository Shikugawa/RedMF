#include <eigen3/Eigen/Core>
#include <vector>
#include <memory>
#include <exception>

template <typename T> class Matrix {
  typedef std::vector<std::vector<T>> TMatrix;

public:
  std::shared_ptr<Eigen::MatrixXd> matrix;

  Matrix(TMatrix R) {
    rowNum = R.size();
    colNum = R[0].size();
    matrix = std::make_shared<Eigen::MatrixXd>(Eigen::Map<Eigen::MatrixXd>(&R[0][0], rowNum, colNum));
  }

  inline int getRowNum() const {
    return rowNum;
  };

  inline int getColNum() const {
    return colNum;
  };

  inline TMatrix getMatrix() {
    return eigenMatrixToSTLVector(colNum, rowNum);
  };

  inline T getMatrixElem(int x, int y) const {
    auto tmp = matrix.get();
    return tmp->coeff(x, y);
  };

  inline void changeElem(int x, int y, T value) {
    auto& tmp = *matrix;
    tmp(x, y) = value;
  }

  std::vector<T> getMatrixRow(int x) {
    if(x >= getRowNum()) throw "invalid argument";
    return eigenVectorToSTLVector((Eigen::VectorXd)matrix.get()->row(x), rowNum);
  }

  std::vector<T> getMatrixCol(int y) {
    if(y >= getColNum()) throw "invalid argument";
    return eigenVectorToSTLVector((Eigen::VectorXd)matrix.get()->col(y), colNum);
  };

  TMatrix getTranspose() {
    TMatrix pTransposed;
    for(size_t i = 0; i < rowNum; i++) {
      std::vector<T> v;
      for(size_t j = 0; j < colNum; j++) {
        v.push_back(matrix.get()->coeff(j, i));
      }
      pTransposed.push_back(v);
    }
    return pTransposed;
  };
  
private:
  int colNum, rowNum;

  TMatrix getFilledMatrix(int num) {
    TMatrix filled;
    for(size_t i = 0; i < getRowNum(); ++i) {
      filled.push_back({});     
      for(size_t j = 0; j < getColNum(); ++j)
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

  std::vector<T> eigenVectorToSTLVector(Eigen::VectorXd const vector, int size) {
    std::vector<T> result;
    for(size_t i = 0; i < size; i++) {
      result.push_back(vector(i));
    }
    return result;
  }
};
