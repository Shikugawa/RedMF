#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include "src/mf.hpp"

std::vector<double> split(std::string const &s, char delimiter) {
  std::vector<double> result;
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delimiter)) {
    if(!item.empty()) {
      result.push_back(std::stod(item));
    }
  }
  return result;
} 

int main(int argc, char const *argv[]) {
  std::vector<std::vector<double>> d;
  std::fstream fs("data.csv");
  std::string line;
  
  while(std::getline(fs, line)) {
    d.push_back(split(line, ','));
  }

  Matrix<double>* m = new Matrix<double>(std::move(d));
  MatrixFactorization<double>* mf = new MatrixFactorization<double>(
    std::move(m), 0.001, 2 
  );

  mf->execute(true);
  delete mf;
  return 0;
}
