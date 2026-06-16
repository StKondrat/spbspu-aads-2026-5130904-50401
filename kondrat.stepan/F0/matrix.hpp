#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cstddef>
#include <iosfwd>
#include <vector/top-it-vector.hpp>

namespace kondrat
{
  using ll = long long;

  class Matrix
  {
    public:
      Matrix();
      Matrix(size_t rows, size_t cols);

      size_t rows() const noexcept;
      size_t cols() const noexcept;

      ll & at(size_t row, size_t col);
      const ll & at(size_t row, size_t col) const;

      bool operator==(const Matrix & rhs) const;
      bool operator!=(const Matrix & rhs) const;

    private:
      size_t rows_;
      size_t cols_;
      topit::Vector< ll > data_;

      size_t index(size_t row, size_t col) const;
  };

  std::ostream & operator<<(std::ostream & out, const Matrix & matrix);
}

#endif
