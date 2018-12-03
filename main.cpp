#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <iterator>
// #include "src/mf.hpp"
#include "src/kmf.hpp"
#include "lib/kernel.hpp"

using TMatrix = std::vector<std::vector<double>>;

std::vector<double> split(std::string const &s, char delimiter) {
  std::vector<double> result;
  std::stringstream ss(s);
  std::string item;
  while(std::getline(ss, item, delimiter)) {
    if(!item.empty()) {
      result.emplace_back(std::stod(item));
    }
  }
  return result;
} 

void outputCSV(std::string const fileName, TMatrix& m) {
  std::ofstream fs(fileName);

  for(auto& i: m) {
    std::ostringstream os;
    std::copy(i.begin(), i.end(), std::ostream_iterator<double>(os, ","));
    std::string s = os.str();
    s.erase(s.size() - std::char_traits<char>::length(","));
    fs << s << std::endl;
  }
}

int main(int argc, char const *argv[]) {
  TMatrix d;
  // TMatrix d = {
  //   {1.0, 4.2, 0.0, 1.9, 2.3},
  //   {2.3, 5.6, 6.3, 0.0, 0.0},
  //   {0.0, 0.0, 2.5, 2.3, 0.0},
  //   {5.2, 1.3, 2.5, 0.0, 2.4},
  //   {0.0, 0.0, 0.0, 2.3, 2.6}
  // };

  {
    std::ifstream fs("./dataset/matrix.csv");
    std::string line;
    
    while(std::getline(fs, line)) {
      d.emplace_back(split(line, ','));
    }
  }

  std::unique_ptr<MF::KernelizedMF<double>> kmf = std::make_unique<MF::KernelizedMF<double>>(
    std::make_unique<Matrix<double>>(d), 5, 5, 0.001, 0.0002, KernelFunctions<double>::gaussianKernel
  );

  kmf->execute(10);

  TMatrix P = kmf->getPMatrix();
  TMatrix Q = kmf->getQMatrix();

  outputCSV("P.csv", P);
  outputCSV("Q.csv", Q);

  return 0;
}
