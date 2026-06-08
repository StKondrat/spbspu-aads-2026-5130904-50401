#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include <utility>
#include "list.hpp"

namespace
{
  using pairN = std::pair< std::string, kondrat::List< size_t > >;

  void getData(std::istream & in, kondrat::List< pairN > & res)
  {
    std::string name;

    while (in >> name)
    {
      kondrat::List< size_t > nums;
      size_t num = 0;

      while (in >> num)
      {
        nums.pushBack(num);
      }

      pairN p(name, nums);
      res.pushBack(p);

      if (in.fail() && !in.eof())
      {
        in.clear();
      }
    }
  }

  void printNames(const kondrat::List< pairN > & data)
  {
    kondrat::LCIter< pairN > it = data.begin();
    kondrat::LCIter< pairN > end = data.end();

    if (it != end)
    {
      std::cout << it->first;
      ++it;
    }

    while (it != end)
    {
      std::cout << ' ' << it->first;
      ++it;
    }
  }

  bool hasNumbers(const kondrat::List< pairN > & data)
  {
    kondrat::LCIter< pairN > it = data.begin();
    kondrat::LCIter< pairN > end = data.end();

    while (it != end)
    {
      if (!(it->second.empty()))
      {
        return true;
      }
      ++it;
    }

    return false;
  }

  void printRow(const kondrat::List< size_t > & row)
  {
    kondrat::LCIter< size_t > it = row.begin();
    kondrat::LCIter< size_t > end = row.end();

    if (it != end)
    {
      std::cout << *it;
      ++it;
    }

    while (it != end)
    {
      std::cout << ' ' << *it;
      ++it;
    }
  }

  size_t formOneRow(kondrat::List< pairN > & data, kondrat::List< size_t > & row)
  {
    kondrat::LIter< pairN > it = data.begin();
    kondrat::LIter< pairN > end = data.end();
    size_t sum = 0;

    while (it != end)
    {
      if (!(it->second.empty()))
      {
        size_t value = it->second.front();

        if (sum > std::numeric_limits< size_t >::max() - value)
        {
          throw std::overflow_error("overflow");
        }

        sum += value;
        row.pushBack(value);
        it->second.popFront();
      }

      ++it;
    }

    return sum;
  }

  void printRows(const kondrat::List< kondrat::List< size_t > > & rows)
  {
    kondrat::LCIter< kondrat::List< size_t > > it = rows.begin();
    kondrat::LCIter< kondrat::List< size_t > > end = rows.end();

    while (it != end)
    {
      printRow(*it);
      std::cout << '\n';
      ++it;
    }
  }

  void printSums(const kondrat::List< size_t > & sums)
  {
    if (sums.empty())
    {
      std::cout << 0;
      return;
    }

    kondrat::LCIter< size_t > it = sums.begin();
    kondrat::LCIter< size_t > end = sums.end();

    std::cout << *it;
    ++it;

    while (it != end)
    {
      std::cout << ' ' << *it;
      ++it;
    }
  }
}

int main()
{
  kondrat::List< pairN > data;

  getData(std::cin, data);

  if (data.empty())
  {
    std::cout << 0 << '\n';
    return 0;
  }

  kondrat::List< pairN > copy(data);
  kondrat::List< kondrat::List< size_t > > rows;
  kondrat::List< size_t > sums;

  try
  {
    while (hasNumbers(copy))
    {
      kondrat::List< size_t > row;
      size_t sum = formOneRow(copy, row);
      rows.pushBack(std::move(row));
      sums.pushBack(sum);
    }
  }
  catch (const std::overflow_error &)
  {
    std::cerr << "ERROR\n";
    return 1;
  }

  printNames(data);
  std::cout << '\n';
  printRows(rows);
  printSums(sums);
  std::cout << '\n';

  return 0;
}
