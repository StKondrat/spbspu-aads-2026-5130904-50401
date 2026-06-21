#ifndef CUCKOO_HASH_ITER_HPP
#define CUCKOO_HASH_ITER_HPP

#include <cstddef>
#include <memory>
#include <vector/top-it-vector.hpp>

namespace kondrat
{
  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class CuckooHashTable;

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class HashIter
  {
    public:
      using Node = typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::Node;

      HashIter();

      Node & operator*() const;
      Node * operator->() const;

      HashIter & operator++();
      HashIter operator++(int);

      bool operator==(const HashIter & other) const;
      bool operator!=(const HashIter & other) const;

    private:
      friend class CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >;

      using Table = CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >;

      Table * table_;
      size_t tableIndex_;
      size_t nodeIndex_;

      HashIter(Table * table, size_t tableIndex, size_t nodeIndex);
      void advance();
  };

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashIter< Key, Value, PrimHash, SecHash, Equal >::HashIter():
    table_(nullptr),
    tableIndex_(2),
    nodeIndex_(0)
  {}

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashIter< Key, Value, PrimHash, SecHash, Equal >::HashIter(
    Table * table,
    size_t tableIndex,
    size_t nodeIndex):
    table_(table),
    tableIndex_(tableIndex),
    nodeIndex_(nodeIndex)
  {
    advance();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename HashIter< Key, Value, PrimHash, SecHash, Equal >::Node &
  HashIter< Key, Value, PrimHash, SecHash, Equal >::operator*() const
  {
    if (tableIndex_ == 0)
    {
      return table_->firstTable_[nodeIndex_];
    }
    return table_->secondTable_[nodeIndex_];
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename HashIter< Key, Value, PrimHash, SecHash, Equal >::Node *
  HashIter< Key, Value, PrimHash, SecHash, Equal >::operator->() const
  {
    return std::addressof(operator*());
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashIter< Key, Value, PrimHash, SecHash, Equal > &
  HashIter< Key, Value, PrimHash, SecHash, Equal >::operator++()
  {
    ++nodeIndex_;
    advance();
    return *this;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashIter< Key, Value, PrimHash, SecHash, Equal >
  HashIter< Key, Value, PrimHash, SecHash, Equal >::operator++(int)
  {
    HashIter copy(*this);
    ++(*this);
    return copy;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool HashIter< Key, Value, PrimHash, SecHash, Equal >::operator==(const HashIter & other) const
  {
    return table_ == other.table_
      && tableIndex_ == other.tableIndex_
      && nodeIndex_ == other.nodeIndex_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool HashIter< Key, Value, PrimHash, SecHash, Equal >::operator!=(const HashIter & other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void HashIter< Key, Value, PrimHash, SecHash, Equal >::advance()
  {
    if (table_ == nullptr || tableIndex_ == 2)
    {
      return;
    }

    while (tableIndex_ < 2)
    {
      const topit::Vector< Node > & table =
        tableIndex_ == 0 ? table_->firstTable_ : table_->secondTable_;
      while (nodeIndex_ < table.getSize())
      {
        if (table[nodeIndex_].occupied_)
        {
          return;
        }
        ++nodeIndex_;
      }
      ++tableIndex_;
      nodeIndex_ = 0;
    }
  }
}

#endif
