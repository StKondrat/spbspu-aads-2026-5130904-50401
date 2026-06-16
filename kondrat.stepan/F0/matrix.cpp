#include "matrix.hpp"
#include <limits>
#include <ostream>
#include <stdexcept>

namespace kondrat
{
  namespace
  {
    size_t getMatrixSize(size_t rows, size_t cols)
    {
      if (rows == 0 || cols == 0)
      {
        throw std::logic_error("invalid matrix size");
      }
      if (rows > std::numeric_limits< size_t >::max() / cols)
      {
        throw std::overflow_error("matrix size overflow");
      }

      return rows * cols;
    }
  }

  Matrix::Matrix():
    rows_(0),
    cols_(0),
    data_()
  {}

  Matrix::Matrix(size_t rows, size_t cols):
    rows_(rows),
    cols_(cols),
    data_(getMatrixSize(rows, cols), 0)
  {}

  size_t Matrix::rows() const noexcept
  {
    return rows_;
  }

  size_t Matrix::cols() const noexcept
  {
    return cols_;
  }

  ll & Matrix::at(size_t row, size_t col)
  {
    return data_[index(row, col)];
  }

  const ll & Matrix::at(size_t row, size_t col) const
  {
    return data_[index(row, col)];
  }

  bool Matrix::operator==(const Matrix & rhs) const
  {
    return rows_ == rhs.rows_ && cols_ == rhs.cols_ && data_ == rhs.data_;
  }

  bool Matrix::operator!=(const Matrix & rhs) const
  {
    return !(*this == rhs);
  }

  size_t Matrix::index(size_t row, size_t col) const
  {
    if (row >= rows_ || col >= cols_)
    {
      throw std::out_of_range("matrix index out of range");
    }

    return row * cols_ + col;
  }

  std::ostream & operator<<(std::ostream & out, const Matrix & matrix)
  {
    for (size_t i = 0; i < matrix.rows(); ++i)
    {
      for (size_t j = 0; j < matrix.cols(); ++j)
      {
        if (j != 0)
        {
          out << ' ';
        }
        out << matrix.at(i, j);
      }
      if (i + 1 < matrix.rows())
      {
        out << '\n';
      }
    }

    return out;
  }
}
