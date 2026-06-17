#include "matrix-const-iter.hpp"
#include <memory>
#include "matrix.hpp"
#include "matrix-iter.hpp"

kondrat::MatrixConstIter::MatrixConstIter():
  matrix_(nullptr),
  index_(0)
{}

kondrat::MatrixConstIter::MatrixConstIter(const MatrixIter & other):
  matrix_(other.matrix_),
  index_(other.index_)
{}

kondrat::MatrixConstIter::MatrixConstIter(const Matrix * matrix, size_t index):
  matrix_(matrix),
  index_(index)
{}

const kondrat::ll & kondrat::MatrixConstIter::operator*() const
{
  return matrix_->data_[index_];
}

const kondrat::ll * kondrat::MatrixConstIter::operator->() const
{
  return std::addressof(operator*());
}

kondrat::MatrixConstIter & kondrat::MatrixConstIter::operator++()
{
  ++index_;
  return *this;
}

kondrat::MatrixConstIter kondrat::MatrixConstIter::operator++(int)
{
  MatrixConstIter copy(*this);
  ++(*this);
  return copy;
}

bool kondrat::MatrixConstIter::operator==(const MatrixConstIter & other) const
{
  return matrix_ == other.matrix_ && index_ == other.index_;
}

bool kondrat::MatrixConstIter::operator!=(const MatrixConstIter & other) const
{
  return !(*this == other);
}
