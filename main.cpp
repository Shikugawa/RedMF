#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <iterator>
#include "src/mf.hpp"

using TMatrix = std::vector<std::vector<double>>;

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

  {
    std::ifstream fs("./dataset/matrix.csv");
    std::string line;
    
    while(std::getline(fs, line)) {
      d.push_back(split(line, ','));
    }
  }

  
  std::unique_ptr<MF::MatrixFactorization<double>> mf = std::make_unique<MF::MatrixFactorization<double>>(
    std::make_unique<Matrix<double>>(d), 0.001, 20
  );
 
  mf->execute(20, true);

  TMatrix P = mf->getPMatrix();
  TMatrix Q = mf->getQMatrix();

  outputCSV("P.csv", P);
  outputCSV("Q.csv", Q);

  return 0;
}
