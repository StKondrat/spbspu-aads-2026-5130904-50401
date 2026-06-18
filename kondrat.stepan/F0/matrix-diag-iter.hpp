#ifndef MATRIX_DIAG_ITER_HPP
#define MATRIX_DIAG_ITER_HPP

#include <cstddef>

namespace kondrat
{
  class Matrix;
  class MatrixDiagConstIter;

  class MatrixDiagIter
  {
    public:
      MatrixDiagIter();

      long long & operator*() const;
      long long * operator->() const;

      MatrixDiagIter & operator++();
      MatrixDiagIter operator++(int);

      bool operator==(const MatrixDiagIter & other) const;
      bool operator!=(const MatrixDiagIter & other) const;

    private:
      friend class Matrix;
      friend class MatrixDiagConstIter;

      Matrix * matrix_;
      size_t index_;
      bool sideDiag_;

      MatrixDiagIter(Matrix * matrix, size_t index, bool sideDiag);
  };
}

#endif
