#include <iostream>
#include <vector>
#include <utility>
#include "src/mf.hpp"

int main(int argc, char const *argv[]) {
  Matrix<double>* m = new Matrix<double>(
    {
      {5.0, 3.0, 0.0, 1.0},
      {4.0, 0.0, 0.0, 1.0},
      {1.0, 1.0, 0.0, 5.0},
      {1.0, 0.0, 0.0, 4.0},
      {0.0, 1.0, 5.0, 4.0}
    }
  );

  MatrixFactorization<double>* mf = new MatrixFactorization<double>(
    std::move(m), 0.001, 2 
  );

  mf->execute(true);
  delete mf;
  return 0;
}
