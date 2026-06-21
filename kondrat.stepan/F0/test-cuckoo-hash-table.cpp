#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <stdexcept>
#include "cuckoo-hash-table.hpp"

namespace
{
  struct FirstHash
  {
    size_t operator()(int value) const
    {
      return static_cast< size_t >(value);
    }
  };

  struct SecondHash
  {
    size_t operator()(int value) const
    {
      return static_cast< size_t >(value * 7 + 3);
    }
  };

  using Table = kondrat::CuckooHashTable< int, int, FirstHash, SecondHash >;
}

BOOST_AUTO_TEST_SUITE(CuckooHashTableTests)

BOOST_AUTO_TEST_CASE(DefaultConstruction)
{
  const Table table;

  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() >= 16);
  BOOST_CHECK(table.cbegin() == table.cend());
}

BOOST_AUTO_TEST_CASE(AddAndGet)
{
  Table table;
  table.add(1, 10);
  table.add(2, 20);
  table.add(3, 30);

  BOOST_CHECK(!table.empty());
  BOOST_CHECK(table.size() == 3);
  BOOST_CHECK(table.has(1));
  BOOST_CHECK(table.has(2));
  BOOST_CHECK(table.has(3));
  BOOST_CHECK(table.get(1) == 10);
  BOOST_CHECK(table.get(2) == 20);
  BOOST_CHECK(table.get(3) == 30);
}

BOOST_AUTO_TEST_CASE(ReplaceValue)
{
  Table table;
  table.add(1, 10);
  table.add(1, 42);

  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(table.get(1) == 42);
}

BOOST_AUTO_TEST_CASE(Drop)
{
  Table table;
  table.add(1, 10);
  table.add(2, 20);

  const int value = table.drop(1);

  BOOST_CHECK(value == 10);
  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(!table.has(1));
  BOOST_CHECK(table.has(2));
}

BOOST_AUTO_TEST_CASE(Clear)
{
  Table table;
  table.add(1, 10);
  table.add(2, 20);
  table.clear();

  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.begin() == table.end());
}

BOOST_AUTO_TEST_CASE(Exceptions)
{
  Table table;

  BOOST_CHECK_THROW(table.get(1), std::logic_error);
  BOOST_CHECK_THROW(table.drop(1), std::logic_error);
  BOOST_CHECK_THROW(table.rehash(0), std::logic_error);
}

BOOST_AUTO_TEST_CASE(Iterators)
{
  Table table;
  table.add(1, 10);
  table.add(2, 20);
  table.add(3, 30);

  size_t count = 0;
  int valueSum = 0;
  int keySum = 0;
  for (Table::Iterator it = table.begin(); it != table.end(); ++it)
  {
    keySum += it->key_;
    valueSum += it->value_;
    ++count;
  }

  BOOST_CHECK(count == table.size());
  BOOST_CHECK(keySum == 6);
  BOOST_CHECK(valueSum == 60);
}

BOOST_AUTO_TEST_CASE(ConstIterators)
{
  Table table;
  table.add(1, 10);
  table.add(2, 20);
  const Table & constTable = table;

  size_t count = 0;
  int valueSum = 0;
  for (Table::ConstIterator it = constTable.cbegin(); it != constTable.cend(); ++it)
  {
    valueSum += it->value_;
    ++count;
  }

  BOOST_CHECK(count == constTable.size());
  BOOST_CHECK(valueSum == 30);
}

BOOST_AUTO_TEST_CASE(Rehash)
{
  Table table;
  for (int i = 0; i < 50; ++i)
  {
    table.add(i, i * 10);
  }

  const size_t oldCapacity = table.capacity();
  table.rehash(oldCapacity * 2);

  BOOST_CHECK(table.size() == 50);
  BOOST_CHECK(table.capacity() >= oldCapacity * 2);
  for (int i = 0; i < 50; ++i)
  {
    BOOST_CHECK(table.get(i) == i * 10);
  }
}

BOOST_AUTO_TEST_CASE(CopyConstructionAndAssignment)
{
  Table source;
  source.add(1, 10);
  source.add(2, 20);

  Table copy(source);
  Table assigned;
  assigned.add(3, 30);
  assigned = source;

  source.add(1, 100);

  BOOST_CHECK(copy.size() == 2);
  BOOST_CHECK(copy.get(1) == 10);
  BOOST_CHECK(copy.get(2) == 20);
  BOOST_CHECK(assigned.size() == 2);
  BOOST_CHECK(assigned.get(1) == 10);
  BOOST_CHECK(assigned.get(2) == 20);
  BOOST_CHECK(!assigned.has(3));
}

BOOST_AUTO_TEST_CASE(Swap)
{
  Table lhs;
  lhs.add(1, 10);
  Table rhs;
  rhs.add(2, 20);
  rhs.add(3, 30);

  lhs.swap(rhs);

  BOOST_CHECK(lhs.size() == 2);
  BOOST_CHECK(lhs.has(2));
  BOOST_CHECK(lhs.has(3));
  BOOST_CHECK(rhs.size() == 1);
  BOOST_CHECK(rhs.has(1));
}

BOOST_AUTO_TEST_SUITE_END()
