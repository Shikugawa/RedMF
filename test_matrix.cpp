#include "lib/matrix.hpp"
#include <iostream>
#include <vector>
#include <memory>

typedef std::vector<std::vector<double>> TMatrix;

void printMatrix(const TMatrix im){
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
  TMatrix vtr = {
    {1.0, 4.2, 0.0, 1.9, 2.3},
    {2.3, 5.6, 6.3, 0.0, 0.0},
    {0.0, 0.0, 2.5, 2.3, 0.0},
    {5.2, 1.3, 2.5, 0.0, 2.4},
    {0.0, 0.0, 0.0, 2.3, 2.6}
  };
  
  std::unique_ptr<Matrix<double>> pm = std::make_unique<Matrix<double>>(vtr);
  std::cout << "expect 3" << std::endl;
  std::cout << pm->colNum << std::endl;
  std::cout << "expect 2" << std::endl;
  std::cout << pm->rowNum << std::endl;
  std::cout << "expected 2" << std::endl;
  std::cout << pm->getMatrixElem(0, 1) << std::endl;
  pm->changeElem(0, 0, 100);
  std::cout << "expected 100, 2, 8, 3, 4, 5" << std::endl;
  printMatrix(pm->getMatrix());
  std::cout << "expected 3, 4, 5" << std::endl;
  printVector<double>(pm->getMatrixRow(1));
  std::cout << "expected 2, 4" << std::endl;
  printVector<double>(pm->getMatrixCol(1));
  std::cout << "expected 100, 3, 2, 4, 8, 5" << std::endl;
  printMatrix(pm->getTranspose());
}

