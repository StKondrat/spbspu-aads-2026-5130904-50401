#ifndef MATRIX_ROW_ITER_HPP
#define MATRIX_ROW_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixRowConstIter;

  class MatrixRowIter
  {
    public:
      MatrixRowIter();

      long long & operator*() const;
      long long * operator->() const;

      MatrixRowIter & operator++();
      MatrixRowIter operator++(int);

      bool operator==(const MatrixRowIter & other) const;
      bool operator!=(const MatrixRowIter & other) const;

    private:
      friend class Matrix;
      friend class MatrixRowConstIter;

      Matrix * matrix_;
      size_t row_;
      size_t col_;

      MatrixRowIter(Matrix * matrix, size_t row, size_t col);
  };
}

#endif
