#include <boost/test/unit_test.hpp>
#include <string>
#include "hash-table.hpp"
#include "hash-func.hpp"

using ht_t = kondrat::HashTable< std::string, int, kondrat::blake2, kondrat::Equal< std::string > >;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(ConstructorDefault)
{
  ht_t table;

  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() == 16);
}

BOOST_AUTO_TEST_CASE(ConstructorWithCapacity)
{
  ht_t table(8);

  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() == 8);
}

BOOST_AUTO_TEST_CASE(AddHasGet)
{
  ht_t table(8);

  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);

  BOOST_CHECK(table.size() == 3);
  BOOST_CHECK(table.has("a"));
  BOOST_CHECK(table.has("b"));
  BOOST_CHECK(table.has("c"));
  BOOST_CHECK(!table.has("d"));

  BOOST_CHECK(table.get("a") == 1);
  BOOST_CHECK(table.get("b") == 2);
  BOOST_CHECK(table.get("c") == 3);
}

BOOST_AUTO_TEST_CASE(AddUpdatesExistingKey)
{
  ht_t table(8);

  table.add("a", 1);
  table.add("a", 100);

  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(table.get("a") == 100);
}

BOOST_AUTO_TEST_CASE(GetThrowsIfKeyNotFound)
{
  ht_t table(8);

  table.add("a", 1);

  BOOST_CHECK_THROW(table.get("b"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(DropRemovesAndReturnsValue)
{
  ht_t table(8);

  table.add("a", 1);
  table.add("b", 2);

  int value = table.drop("a");

  BOOST_CHECK(value == 1);
  BOOST_CHECK(table.size() == 1);
  BOOST_CHECK(!table.has("a"));
  BOOST_CHECK(table.has("b"));
}

BOOST_AUTO_TEST_CASE(DropThrowsIfKeyNotFound)
{
  ht_t table(8);

  table.add("a", 1);

  BOOST_CHECK_THROW(table.drop("b"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  ht_t table(8);

  table.add("a", 1);
  table.add("b", 2);

  table.clear();

  BOOST_CHECK(table.empty());
  BOOST_CHECK(table.size() == 0);
  BOOST_CHECK(table.capacity() == 8);
  BOOST_CHECK(!table.has("a"));
  BOOST_CHECK(!table.has("b"));
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  ht_t table(8);

  table.add("a", 1);
  table.add("b", 2);

  ht_t copy(table);

  BOOST_CHECK(copy.size() == 2);
  BOOST_CHECK(copy.get("a") == 1);
  BOOST_CHECK(copy.get("b") == 2);

  copy.add("a", 100);

  BOOST_CHECK(table.get("a") == 1);
  BOOST_CHECK(copy.get("a") == 100);
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  ht_t table(8);

  table.add("a", 1);
  table.add("b", 2);

  ht_t copy;
  copy = table;

  BOOST_CHECK(copy.size() == 2);
  BOOST_CHECK(copy.get("a") == 1);
  BOOST_CHECK(copy.get("b") == 2);

  copy.drop("a");

  BOOST_CHECK(table.has("a"));
  BOOST_CHECK(!copy.has("a"));
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  ht_t table(8);

  table.add("a", 1);
  table.add("b", 2);

  ht_t moved(std::move(table));

  BOOST_CHECK(moved.size() == 2);
  BOOST_CHECK(moved.get("a") == 1);
  BOOST_CHECK(moved.get("b") == 2);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  ht_t table(8);

  table.add("a", 1);
  table.add("b", 2);

  ht_t moved;
  moved = std::move(table);

  BOOST_CHECK(moved.size() == 2);
  BOOST_CHECK(moved.get("a") == 1);
  BOOST_CHECK(moved.get("b") == 2);
}

BOOST_AUTO_TEST_CASE(Rehash)
{
  ht_t table(8);

  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);

  table.rehash(32);

  BOOST_CHECK(table.capacity() == 32);
  BOOST_CHECK(table.size() == 3);
  BOOST_CHECK(table.get("a") == 1);
  BOOST_CHECK(table.get("b") == 2);
  BOOST_CHECK(table.get("c") == 3);
}

BOOST_AUTO_TEST_CASE(IteratorWalk)
{
  ht_t table(8);

  table.add("a", 1);
  table.add("b", 2);
  table.add("c", 3);
  table.drop("b");

  size_t count = 0;
  int sum = 0;

  for (kondrat::HTIter< std::string, int > it = table.begin(); it != table.end(); ++it)
  {
    ++count;
    sum += it->value;
  }

  BOOST_CHECK(count == 2);
  BOOST_CHECK(sum == 4);
}

BOOST_AUTO_TEST_SUITE_END()
