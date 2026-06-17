#include "matrix-row-const-iter.hpp"
#include <memory>
#include "matrix.hpp"
#include "matrix-row-iter.hpp"

kondrat::MatrixRowConstIter::MatrixRowConstIter():
  matrix_(nullptr),
  row_(0),
  col_(0)
{}

kondrat::MatrixRowConstIter::MatrixRowConstIter(const MatrixRowIter & other):
  matrix_(other.matrix_),
  row_(other.row_),
  col_(other.col_)
{}

kondrat::MatrixRowConstIter::MatrixRowConstIter(const Matrix * matrix, size_t row, size_t col):
  matrix_(matrix),
  row_(row),
  col_(col)
{}

const long long & kondrat::MatrixRowConstIter::operator*() const
{
  return matrix_->data_[row_ * matrix_->cols_ + col_];
}

const long long * kondrat::MatrixRowConstIter::operator->() const
{
  return std::addressof(operator*());
}

kondrat::MatrixRowConstIter & kondrat::MatrixRowConstIter::operator++()
{
  ++col_;
  return *this;
}

kondrat::MatrixRowConstIter kondrat::MatrixRowConstIter::operator++(int)
{
  MatrixRowConstIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixRowConstIter::operator==(const MatrixRowConstIter & other) const
{
  return matrix_ == other.matrix_ && row_ == other.row_ && col_ == other.col_;
}

bool kondrat::MatrixRowConstIter::operator!=(const MatrixRowConstIter & other) const
{
  return !(*this == other);
}
