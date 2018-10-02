#include <vector>
#include <optional>

template <class T>
class Matrix {
public:
  std::vector<std::vector<T>> matrix;
  int colNum, rowNum;

  Matrix(std::vector<std::vector<T>> R) {
    this->matrix = R;
    this->rowNum = this->matrix.count();
    this->colNum = this->matrix[0].count();
  }

  ~Matrix() {
    std::vector<T>().swap(this->matrix);
  }

  int getRowNum() {
    return this->rowNum;
  };

  int getColNum() {
    return this->colNum;
  };

  std::vector<std::vector<T>> getMatrix() {
    return this->matrix;
  };

  T getMatrixElem(int x, int y) {
    return this->matrix[x][y];
  };

  std::optional<std::vector<T>> Matrix::getMatrixRow(int x) {
    if(x > colNum) return;
    return this->matrix[x];
  }

  std::optional<std::vector<T>> getMatrixCol(int y) {
    if(y > this->getRowNum()) return;
    std::vector<T> vec;
    for(size_t i = 0; i < this->getRowNum(); i++)
      vec.push_back(this->matrix[i][y]);
    
    return vec;
  };

  std::vector<T> getTranspose() {
    Matrix* transposed = new Matrix<T>(this->getFilledMatrix(0));
    for(size_t i = 0; i < this->getRowNum(); i++)
      for(size_t j = 0; j < this->getColNum(); j++)
        transposed[i][j] = this->matrix[j][i];
    
    return transposed;
  };
  
private:
  std::vector<std::vector<T>> getFilledMatrix(int num) {
    std::vector<std::vector<T>> filled;
    for(size_t i = 0; i < this->getRowNum(); i++) {
      filled.push_back({});     
      for(size_t j = 0; j < this->getColNum(); j++)
        filled[i].push_bach(0);
    }

    return filled;
  };
};
