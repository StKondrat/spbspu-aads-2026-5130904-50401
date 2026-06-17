#include <memory>
#include "matrix-iter.hpp"
#include "matrix.hpp"

kondrat::MatrixIter::MatrixIter():
  matrix_(nullptr),
  index_(0)
{}

kondrat::MatrixIter::MatrixIter(Matrix * matrix, size_t index):
  matrix_(matrix),
  index_(index)
{}

kondrat::ll & kondrat::MatrixIter::operator*() const
{
  return matrix_->data_[index_];
}

kondrat::ll * kondrat::MatrixIter::operator->() const
{
  return std::addressof(operator*());
}

kondrat::MatrixIter & kondrat::MatrixIter::operator++()
{
  ++index_;
  return *this;
}

kondrat::MatrixIter kondrat::MatrixIter::operator++(int)
{
  MatrixIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixIter::operator==(const MatrixIter & other) const
{
  return matrix_ == other.matrix_ && index_ == other.index_;
}

bool kondrat::MatrixIter::operator!=(const MatrixIter & other) const
{
  return !(*this == other);
}
