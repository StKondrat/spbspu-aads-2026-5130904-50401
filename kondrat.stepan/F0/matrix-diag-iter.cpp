#include "matrix-diag-iter.hpp"
#include <memory>
#include "matrix.hpp"

kondrat::MatrixDiagIter::MatrixDiagIter():
  matrix_(nullptr),
  index_(0),
  sideDiag_(false)
{}

kondrat::MatrixDiagIter::MatrixDiagIter(Matrix * matrix, size_t index, bool sideDiag):
  matrix_(matrix),
  index_(index),
  sideDiag_(sideDiag)
{}

long long & kondrat::MatrixDiagIter::operator*() const
{
  const size_t col = sideDiag_ ? matrix_->cols_ - index_ - 1 : index_;
  return matrix_->data_[index_ * matrix_->cols_ + col];
}

long long * kondrat::MatrixDiagIter::operator->() const
{
  return std::addressof(operator*());
}

kondrat::MatrixDiagIter & kondrat::MatrixDiagIter::operator++()
{
  ++index_;
  return *this;
}

kondrat::MatrixDiagIter kondrat::MatrixDiagIter::operator++(int)
{
  MatrixDiagIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixDiagIter::operator==(const MatrixDiagIter & other) const
{
  return matrix_ == other.matrix_ && index_ == other.index_ && sideDiag_ == other.sideDiag_;
}

bool kondrat::MatrixDiagIter::operator!=(const MatrixDiagIter & other) const
{
  return !(*this == other);
}
