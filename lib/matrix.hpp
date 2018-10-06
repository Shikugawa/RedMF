#include <vector>
#include <memory>
#include <exception>

template <typename T> class Matrix {
  typedef std::vector<std::vector<T>> TMatrix;

public:
  Matrix(std::vector<std::vector<T>> R) {
    this->matrix = R;
    this->rowNum = this->matrix.size();
    this->colNum = this->matrix[0].size();
  }

  inline int getRowNum() const {
    return this->rowNum;
  };

  inline int getColNum() const {
    return this->colNum;
  };

  inline TMatrix getMatrix() const {
    return this->matrix;
  };

  inline T getMatrixElem(int x, int y) const {
    return this->matrix[x][y];
  };

  inline void changeElem(int x, int y, T value) {
    this->matrix[x][y] = value;
  }

  std::vector<T> getMatrixRow(int x) const {
    if(x >= this->getRowNum()) throw "invalid argument";
    return this->matrix[x];
  }

  std::vector<T> getMatrixCol(int y) {
    if(y >= this->getColNum()) throw "invalid argument";
    std::vector<T> vec;
    for(size_t i = 0; i < this->getRowNum(); ++i)
      vec.push_back(this->matrix[i][y]);
    
    return vec;
  };

  TMatrix getTranspose() {
    std::unique_ptr<Matrix> pTransposed = std::make_unique<Matrix<T>>(this->getFilledMatrix(0));
    for(size_t i = 0; i < this->getRowNum(); ++i)
      for(size_t j = 0; j < this->getColNum(); ++j)
        pTransposed->changeElem(i, j, this->getMatrixElem(j, i));
    
    return pTransposed->getMatrix();
  };
  
private:
  TMatrix matrix;
  int colNum, rowNum;

  TMatrix getFilledMatrix(int num) {
    TMatrix filled;
    for(size_t i = 0; i < this->getRowNum(); ++i) {
      filled.push_back({});     
      for(size_t j = 0; j < this->getColNum(); ++j)
        filled[i].push_back(0);
    }

    return filled;
  };
};
