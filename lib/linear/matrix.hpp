#pragma once
#include <eigen3/Eigen/Core>
#include <vector>
#include <memory>
#include <exception>

namespace RedMF::Linear {
  template <typename T> 
  class Matrix {
    using TMatrix = std::vector<std::vector<T>>;

  public:
    std::shared_ptr<Eigen::MatrixXd> matrix;
    int colNum, rowNum;

    Matrix(TMatrix R) {
      rowNum = R.size();
      colNum = R[0].size();
      tMatrixToEigenMatrix(R);
    }

    Matrix(int rowSize, int colSize) {
      rowNum = rowSize;
      colNum = colSize;
      matrix = std::make_unique<Eigen::MatrixXd>(Eigen::MatrixXd::Random(rowNum, colNum).cwiseAbs());
    }

    Matrix* operator*(const Matrix& m) {
      auto matrixLeft = *(this->matrix);
      auto matrixRight = *(m.matrix);
      auto result = eigenMatrixToSTLVector(matrixLeft * matrixRight);
      return new Matrix(result);
    }

    inline TMatrix getMatrix() {
      return eigenMatrixToSTLVector();
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
      assert(x < rowNum);
      auto passingVector = static_cast<Eigen::VectorXd>(matrix->row(x));
      return eigenVectorToSTLVector(passingVector);
    }

    std::vector<T> getMatrixCol(int y) {
      assert(y < colNum);
      auto passingVector = static_cast<Eigen::VectorXd>(matrix->col(y));
      return eigenVectorToSTLVector(passingVector);
    };

    TMatrix getTranspose() {
      TMatrix pTransposed;
      for(size_t i = 0; i < colNum; i++) {
        std::vector<T> v;
        for(size_t j = 0; j < rowNum; j++) {
          v.emplace_back(matrix->coeff(j, i));
        }
        pTransposed.emplace_back(v);
      }
      return pTransposed;
    };

    static TMatrix eigenMatrixToSTLVector(const Eigen::MatrixXd& m) {
      TMatrix result;
      for(size_t i = 0; i < m.rows(); ++i) {
        std::vector<T> v;
        for(size_t j = 0; j < m.cols(); ++j) {
          v.emplace_back(m.coeff(i, j));
        }
        result.emplace_back(v);
      }
      return result;
    }

  private:
    TMatrix getFilledMatrix(int num) {
      TMatrix filled;
      for(size_t i = 0; i < rowNum; ++i) {
        filled.emplace_back({});     
        for(size_t j = 0; j < colNum; ++j)
          filled[i].emplace_back(0);
      }

      return filled;
    };

    TMatrix eigenMatrixToSTLVector() {
      TMatrix result;
      for(size_t i = 0; i < this->rowNum; ++i) {
        std::vector<T> v;
        for(size_t j = 0; j < this->colNum; ++j) {
          v.emplace_back(matrix.get()->coeff(i, j));
        }
        result.emplace_back(v);
      }
      return result;
    }

    std::vector<T> eigenVectorToSTLVector(Eigen::VectorXd vector) {
      std::vector<T> result;
      for(size_t i = 0; i < vector.size(); i++) {
        result.emplace_back(vector.coeff(i));
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
}
