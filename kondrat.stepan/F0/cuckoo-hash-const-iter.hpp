#ifndef CUCKOO_HASH_CONST_ITER_HPP
#define CUCKOO_HASH_CONST_ITER_HPP

#include <cstddef>
#include <memory>

namespace kondrat
{
  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class CuckooHashTable;

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  class HashConstIter
  {
    public:
      using table_type = CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >;
      using Node = typename table_type::Node;

      HashConstIter();
      HashConstIter(const table_type * table, size_t tableIndex, size_t nodeIndex);

      const Node & operator*() const;
      const Node * operator->() const;

      HashConstIter & operator++();
      HashConstIter operator++(int);

      bool operator==(const HashConstIter & other) const;
      bool operator!=(const HashConstIter & other) const;

    private:
      const table_type * table_;
      size_t tableIndex_;
      size_t nodeIndex_;

      void advance();
  };

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::HashConstIter():
    table_(nullptr),
    tableIndex_(2),
    nodeIndex_(0)
  {}

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::HashConstIter(
    const table_type * table,
    size_t tableIndex,
    size_t nodeIndex):
    table_(table),
    tableIndex_(tableIndex),
    nodeIndex_(nodeIndex)
  {
    advance();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  const typename HashConstIter< Key, Value, PrimHash, SecHash, Equal >::Node &
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator*() const
  {
    if (tableIndex_ == 0)
    {
      return table_->firstTable_[nodeIndex_];
    }

    return table_->secondTable_[nodeIndex_];
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  const typename HashConstIter< Key, Value, PrimHash, SecHash, Equal >::Node *
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator->() const
  {
    return std::addressof(operator*());
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashConstIter< Key, Value, PrimHash, SecHash, Equal > &
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator++()
  {
    ++nodeIndex_;
    advance();
    return *this;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >
  HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator++(int)
  {
    HashConstIter copy(*this);
    ++(*this);
    return copy;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator==(const HashConstIter & other) const
  {
    return table_ == other.table_ && tableIndex_ == other.tableIndex_ && nodeIndex_ == other.nodeIndex_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool HashConstIter< Key, Value, PrimHash, SecHash, Equal >::operator!=(const HashConstIter & other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void HashConstIter< Key, Value, PrimHash, SecHash, Equal >::advance()
  {
    if (table_ == nullptr || tableIndex_ == 2)
    {
      return;
    }

    while (tableIndex_ < 2)
    {
      const topit::Vector< Node > & table = tableIndex_ == 0 ? table_->firstTable_ : table_->secondTable_;

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
