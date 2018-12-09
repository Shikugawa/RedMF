#include "gurobi_c++.h"

int main(int argc, char const *argv[]){
  try {
    GRBEnv env = GRBEnv();
    GRBModel model = GRBModel(env);
  }
  return 0;
}
