#ifndef MATRIX_ROW_CONST_ITER_HPP
#define MATRIX_ROW_CONST_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixRowIter;

  class MatrixRowConstIter
  {
    public:
      MatrixRowConstIter();
      MatrixRowConstIter(const MatrixRowIter & other);

      const long long & operator*() const;
      const long long * operator->() const;

      MatrixRowConstIter & operator++();
      MatrixRowConstIter operator++(int);

      bool operator==(const MatrixRowConstIter & other) const;
      bool operator!=(const MatrixRowConstIter & other) const;

    private:
      friend class Matrix;

      const Matrix * matrix_;
      size_t row_;
      size_t col_;

      MatrixRowConstIter(const Matrix * matrix, size_t row, size_t col);
  };
}

#endif
