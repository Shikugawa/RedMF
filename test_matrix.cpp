#include <iostream>
#include "matrix/matrix.hpp"
#include <vector>
#include <memory>

typedef std::vector<std::vector<int>> intMatrix;

void printMatrix(const intMatrix im){
  for(auto& i : im){
    for(auto& j: i){
      std::cout << j << std::endl;
    }
  }
}

template<typename T> void printVector(const std::vector<T> v){
  for(auto& i: v){
    std::cout << i << std::endl;
  }
}

int main(void){
  intMatrix vtr = {
      {1, 2},
      {3, 4}
    };
  
  std::unique_ptr<Matrix<int>> pm = std::make_unique<Matrix<int>>(vtr);
  std::cout << "colnum" << std::endl;
  std::cout << pm->getColNum() << std::endl;
  std::cout << "rownum" << std::endl;
  std::cout << pm->getRowNum() << std::endl;
  std::cout << "expected 2" << std::endl;
  std::cout << pm->getMatrixElem(0, 1) << std::endl;
  pm->changeElem(0, 0, 100);
  std::cout << "expected 100, 2, 3, 4" << std::endl;
  printMatrix(pm->getMatrix());
  std::cout << "expected 100, 2" << std::endl;
  printVector<int>(pm->getMatrixRow(0));
  std::cout << "expected 100, 3" << std::endl;
  printVector<int>(pm->getMatrixCol(0));
  std::cout << "expected 100, 3, 2, 4" << std::endl;
  printMatrix(pm->getTranspose());
}

