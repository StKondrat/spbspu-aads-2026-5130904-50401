#ifndef MATRIX_DIAG_CONST_ITER_HPP
#define MATRIX_DIAG_CONST_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixDiagIter;

  class MatrixDiagConstIter
  {
    public:
      MatrixDiagConstIter();
      MatrixDiagConstIter(const MatrixDiagIter & other);

      const long long & operator*() const;
      const long long * operator->() const;

      MatrixDiagConstIter & operator++();
      MatrixDiagConstIter operator++(int);

      bool operator==(const MatrixDiagConstIter & other) const;
      bool operator!=(const MatrixDiagConstIter & other) const;

    private:
      friend class Matrix;

      const Matrix * matrix_;
      size_t index_;
      bool sideDiag_;

      MatrixDiagConstIter(const Matrix * matrix, size_t index, bool sideDiag);
  };
}

#endif
