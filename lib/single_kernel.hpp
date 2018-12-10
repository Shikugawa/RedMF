#include <memory>
#include "matrix.hpp"

template <typename T>
using kernelFuncType = std::function<T(std::vector<T>, std::vector<T>)>;

template <typename T>
class SingleKernel {
public:
  kernelFuncType<T> kernel_func;
  std::unique_ptr<Matrix<T>> K;
  std::size_t k;
  
  SingleKernel(kernelFuncType<T> _kernel_func, std::shared_ptr<Matrix<T>>& _D, std::size_t _k):
               kernel_func(_kernel_func), k(_k) {
    K = std::make_unique<Matrix<T>>(k, k);
    calcGramMatrix(_D);
  }

private:
  void calcGramMatrix(std::shared_ptr<Matrix<T>>& _D) {
    for(std::size_t i = 0; i < k; ++i) {
      for(std::size_t j = 0; j < k; ++j) {
        auto gramMatrixElem = kernel_func(
          _D->getMatrixCol(i),
          _D->getMatrixCol(j) 
        );
        K->changeElem(i, j, gramMatrixElem);
      }
    }
  }
};
