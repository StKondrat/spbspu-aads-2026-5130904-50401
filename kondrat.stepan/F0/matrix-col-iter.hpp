#ifndef MATRIX_COL_ITER_HPP
#define MATRIX_COL_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixColConstIter;

  class MatrixColIter
  {
    public:
      MatrixColIter();

      long long & operator*() const;
      long long * operator->() const;

      MatrixColIter & operator++();
      MatrixColIter operator++(int);

      bool operator==(const MatrixColIter & other) const;
      bool operator!=(const MatrixColIter & other) const;

    private:
      friend class Matrix;
      friend class MatrixColConstIter;

      Matrix * matrix_;
      size_t row_;
      size_t col_;

      MatrixColIter(Matrix * matrix, size_t row, size_t col);
  };
}

#endif
