#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector/top-it-vector.hpp>
#include "matrix-table.hpp"

namespace kondrat
{
  namespace
  {
    template< class T >
    void sortVector(topit::Vector< T > & values)
    {
      for (size_t i = 0; i < values.getSize(); ++i)
      {
        for (size_t j = i + 1; j < values.getSize(); ++j)
        {
          if (values[j] < values[i])
          {
            T tmp = values[i];
            values[i] = values[j];
            values[j] = tmp;
          }
        }
      }
    }
  }

  void MatrixTable::create(std::istream & in, std::ostream &, std::string matrixName)
  {
    size_t rows = 0;
    size_t cols = 0;

    if (!(in >> rows >> cols) || hasExtraArgs(in) || matrices_.has(matrixName))
    {
      throw std::logic_error("invalid command");
    }

    matrices_.add(matrixName, Matrix(rows, cols));
  }

  void MatrixTable::drop(std::istream & in, std::ostream &, std::string matrixName)
  {
    if (hasExtraArgs(in))
    {
      throw std::logic_error("invalid command");
    }

    matrices_.drop(matrixName);
  }

  void MatrixTable::show(std::istream & in, std::ostream & out, std::string matrixName)
  {
    if (hasExtraArgs(in))
    {
      throw std::logic_error("invalid command");
    }

    out << matrixName << ":\n" << matrices_.get(matrixName) << '\n';
  }

  void MatrixTable::list(std::istream & in, std::ostream & out, std::string)
  {
    if (hasExtraArgs(in))
    {
      throw std::logic_error("invalid command");
    }

    if (matrices_.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    topit::Vector< std::string > names;
    for (Storage::ConstIterator it = matrices_.cbegin(); it != matrices_.cend(); ++it)
    {
      names.pushBack(it->key_);
    }

    sortVector(names);

    for (size_t i = 0; i < names.getSize(); ++i)
    {
      const Matrix & matrix = matrices_.get(names[i]);
      out << names[i] << ' ' << matrix.rows() << 'x' << matrix.cols() << '\n';
    }
  }

  void MatrixTable::rename(std::istream & in, std::ostream &, std::string matrixName)
  {
    std::string newName;

    if (!(in >> newName) || hasExtraArgs(in) || matrices_.has(newName))
    {
      throw std::logic_error("invalid command");
    }

    Storage copy(matrices_);
    Matrix matrix = copy.drop(matrixName);
    copy.add(newName, matrix);
    matrices_.swap(copy);
  }

  void MatrixTable::set(std::istream & in, std::ostream &, std::string matrixName)
  {
    size_t row = 0;
    size_t col = 0;
    ll value = 0;

    if (!(in >> row >> col >> value) || hasExtraArgs(in))
    {
      throw std::logic_error("invalid command");
    }

    Matrix matrix = matrices_.get(matrixName);
    matrix.at(row, col) = value;
    matrices_.add(matrixName, matrix);
  }

  bool MatrixTable::hasExtraArgs(std::istream & in) const
  {
    std::string extra;
    return static_cast< bool >(in >> extra);
  }
}
