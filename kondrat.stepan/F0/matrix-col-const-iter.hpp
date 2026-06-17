#ifndef MATRIX_COL_CONST_ITER_HPP
#define MATRIX_COL_CONST_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixColIter;

  class MatrixColConstIter
  {
    public:
      MatrixColConstIter();
      MatrixColConstIter(const MatrixColIter & other);

      const long long & operator*() const;
      const long long * operator->() const;

      MatrixColConstIter & operator++();
      MatrixColConstIter operator++(int);

      bool operator==(const MatrixColConstIter & other) const;
      bool operator!=(const MatrixColConstIter & other) const;

    private:
      friend class Matrix;

      const Matrix * matrix_;
      size_t row_;
      size_t col_;

      MatrixColConstIter(const Matrix * matrix, size_t row, size_t col);
  };
}

#endif
