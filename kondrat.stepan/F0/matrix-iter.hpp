#ifndef MATRIX_ITER_HPP
#define MATRIX_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixConstIter;

  class MatrixIter
  {
    public:
      MatrixIter();

      long long & operator*() const;
      long long * operator->() const;

      MatrixIter & operator++();
      MatrixIter operator++(int);

      bool operator==(const MatrixIter & other) const;
      bool operator!=(const MatrixIter & other) const;

    private:
      friend class Matrix;
      friend class MatrixConstIter;

      Matrix * matrix_;
      size_t index_;

      MatrixIter(Matrix * matrix, size_t index);
  };
}

#endif
