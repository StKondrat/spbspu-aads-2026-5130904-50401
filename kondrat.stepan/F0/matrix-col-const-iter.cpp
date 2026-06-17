#include "matrix-col-const-iter.hpp"
#include <memory>
#include "matrix.hpp"
#include "matrix-col-iter.hpp"

kondrat::MatrixColConstIter::MatrixColConstIter():
  matrix_(nullptr),
  row_(0),
  col_(0)
{}

kondrat::MatrixColConstIter::MatrixColConstIter(const MatrixColIter & other):
  matrix_(other.matrix_),
  row_(other.row_),
  col_(other.col_)
{}

kondrat::MatrixColConstIter::MatrixColConstIter(
  const Matrix * matrix,
  size_t row,
  size_t col):
  matrix_(matrix),
  row_(row),
  col_(col)
{}

const long long & kondrat::MatrixColConstIter::operator*() const
{
  return matrix_->data_[row_ * matrix_->cols_ + col_];
}

const long long * kondrat::MatrixColConstIter::operator->() const
{
  return std::addressof(operator*());
}

kondrat::MatrixColConstIter & kondrat::MatrixColConstIter::operator++()
{
  ++row_;
  return *this;
}

kondrat::MatrixColConstIter kondrat::MatrixColConstIter::operator++(int)
{
  MatrixColConstIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixColConstIter::operator==(const MatrixColConstIter & other) const
{
  return matrix_ == other.matrix_ && row_ == other.row_ && col_ == other.col_;
}

bool kondrat::MatrixColConstIter::operator!=(const MatrixColConstIter & other) const
{
  return !(*this == other);
}
