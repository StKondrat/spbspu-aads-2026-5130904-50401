#ifndef MATRIX_CONST_ITER_HPP
#define MATRIX_CONST_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixIter;

  class MatrixConstIter
  {
    public:
      MatrixConstIter();
      MatrixConstIter(const MatrixIter & other);

      const long long & operator*() const;
      const long long * operator->() const;

      MatrixConstIter & operator++();
      MatrixConstIter operator++(int);

      bool operator==(const MatrixConstIter & other) const;
      bool operator!=(const MatrixConstIter & other) const;

    private:
      friend class Matrix;

      const Matrix * matrix_;
      size_t index_;

      MatrixConstIter(const Matrix * matrix, size_t index);
  };
}

#endif
