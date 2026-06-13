#ifndef CUCKOO_HASH_TABLE_HPP
#define CUCKOO_HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector/top-it-vector.hpp>
#include "cuckoo-hash-node.hpp"

namespace kondrat
{
  template< class T >
  struct Equal
  {
    bool operator()(const T & lhs, const T & rhs) const;
  };

  template< class Key, class Value, class PrimHash, class SecHash, class Equal = Equal< Key > >
  class CuckooHashTable;

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class HashIter;

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class HashConstIter;

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class CuckooHashTable
  {
    public:
      using Node = CuckooHashNode< Key, Value >;
      using Iterator = HashIter< Key, Value, PrimHash, SecHash, Equal >;
      using ConstIterator = HashConstIter< Key, Value, PrimHash, SecHash, Equal >;

      CuckooHashTable();
      explicit CuckooHashTable(size_t capacity);

      void add(const Key & key, const Value & value);
      Value drop(const Key & key);

      bool has(const Key & key) const;
      Value & get(const Key & key);
      const Value & get(const Key & key) const;

      void clear();
      void rehash(size_t newCapacity);
      void swap(CuckooHashTable & other) noexcept;

      size_t size() const noexcept;
      size_t capacity() const noexcept;
      bool empty() const noexcept;

      Iterator begin() noexcept;
      Iterator end() noexcept;
      ConstIterator begin() const noexcept;
      ConstIterator end() const noexcept;
      ConstIterator cbegin() const noexcept;
      ConstIterator cend() const noexcept;

    private:
      friend class HashIter< Key, Value, PrimHash, SecHash, Equal >;
      friend class HashConstIter< Key, Value, PrimHash, SecHash, Equal >;

      static const size_t minSubtableCapacity_ = 8;
      static const size_t maxLoadFactorMultiplier_ = 2;

      topit::Vector< Node > firstTable_;
      topit::Vector< Node > secondTable_;
      size_t size_;
      PrimHash primaryHash_;
      SecHash secondaryHash_;
      Equal equal_;

      size_t firstIndex(const Key & key) const;
      size_t secondIndex(const Key & key) const;
      size_t subtableCapacity() const noexcept;

      Node * findNode(const Key & key);
      const Node * findNode(const Key & key) const;

      void placeWithoutRehash(const Node & node);
      void rehashAndPlace(const Node & node);
  };
}
