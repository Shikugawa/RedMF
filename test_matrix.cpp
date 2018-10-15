#include "lib/matrix.hpp"
#include <iostream>
#include <vector>
#include <memory>

typedef std::vector<std::vector<double>> doubleMatrix;

void printMatrix(const doubleMatrix im){
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
  doubleMatrix vtr = {
    {1.0, 2.0},
    {3.0, 4.0}
  };
  
  std::unique_ptr<Matrix<double>> pm = std::make_unique<Matrix<double>>(vtr);
  std::cout << "colnum" << std::endl;
  std::cout << pm->colNum << std::endl;
  std::cout << "rownum" << std::endl;
  std::cout << pm->rowNum << std::endl;
  std::cout << "expected 2" << std::endl;
  std::cout << pm->getMatrixElem(0, 1) << std::endl;
  pm->changeElem(0, 0, 100);
  std::cout << "expected 100, 2, 3, 4" << std::endl;
  printMatrix(pm->getMatrix());
  std::cout << "expected 100, 2" << std::endl;
  printVector<double>(pm->getMatrixRow(0));
  std::cout << "expected 100, 3" << std::endl;
  printVector<double>(pm->getMatrixCol(0));
  std::cout << "expected 100, 3, 2, 4" << std::endl;
  printMatrix(pm->getTranspose());
}

