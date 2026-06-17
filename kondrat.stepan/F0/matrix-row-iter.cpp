#include "matrix-row-iter.hpp"
#include <memory>
#include "matrix.hpp"

kondrat::MatrixRowIter::MatrixRowIter():
  matrix_(nullptr),
  row_(0),
  col_(0)
{}

kondrat::MatrixRowIter::MatrixRowIter(Matrix * matrix, size_t row, size_t col):
  matrix_(matrix),
  row_(row),
  col_(col)
{}

long long & kondrat::MatrixRowIter::operator*() const
{
  return matrix_->data_[row_ * matrix_->cols_ + col_];
}

long long * kondrat::MatrixRowIter::operator->() const
{
  return std::addressof(operator*());
}

kondrat::MatrixRowIter & kondrat::MatrixRowIter::operator++()
{
  ++col_;
  return *this;
}

kondrat::MatrixRowIter kondrat::MatrixRowIter::operator++(int)
{
  MatrixRowIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixRowIter::operator==(const MatrixRowIter & other) const
{
  return matrix_ == other.matrix_ && row_ == other.row_ && col_ == other.col_;
}

bool kondrat::MatrixRowIter::operator!=(const MatrixRowIter & other) const
{
  return !(*this == other);
}
