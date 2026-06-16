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
  struct KeyEqual
  {
    bool operator()(const T & lhs, const T & rhs) const;
  };

  template< class Key, class Value, class PrimHash, class SecHash, class Equal = KeyEqual< Key > >
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

  template< class T >
  bool KeyEqual< T >::operator()(const T & lhs, const T & rhs) const
  {
    return lhs == rhs;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::CuckooHashTable():
    CuckooHashTable(minSubtableCapacity_ * maxLoadFactorMultiplier_)
  {}

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::CuckooHashTable(size_t capacity):
    firstTable_(capacity / 2 < minSubtableCapacity_ ? minSubtableCapacity_ : capacity / 2, Node()),
    secondTable_(capacity / 2 < minSubtableCapacity_ ? minSubtableCapacity_ : capacity / 2, Node()),
    size_(0),
    primaryHash_(),
    secondaryHash_(),
    equal_()
  {
    if (capacity == 0)
    {
      throw std::logic_error("invalid capacity");
    }
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::add(const Key & key, const Value & value)
  {
    Node * existing = findNode(key);
    if (existing != nullptr)
    {
      existing->value_ = value;
      return;
    }

    if ((size_ + 1) * maxLoadFactorMultiplier_ > capacity())
    {
      rehash(capacity() * maxLoadFactorMultiplier_);
    }

    rehashAndPlace(Node(key, value));
    ++size_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  Value CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::drop(const Key & key)
  {
    Node * node = findNode(key);
    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    Value value = node->value_;
    *node = Node();
    --size_;
    return value;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::has(const Key & key) const
  {
    return findNode(key) != nullptr;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  Value & CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::get(const Key & key)
  {
    Node * node = findNode(key);
    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    return node->value_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  const Value & CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::get(const Key & key) const
  {
    const Node * node = findNode(key);
    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    return node->value_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::clear()
  {
    for (size_t i = 0; i < firstTable_.getSize(); ++i)
    {
      firstTable_[i] = Node();
      secondTable_[i] = Node();
    }

    size_ = 0;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::rehash(size_t newCapacity)
  {
    if (newCapacity == 0)
    {
      throw std::logic_error("invalid capacity");
    }

    CuckooHashTable fresh(newCapacity);

    for (ConstIterator it = cbegin(); it != cend(); ++it)
    {
      fresh.add(it->key_, it->value_);
    }

    swap(fresh);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::swap(CuckooHashTable & other) noexcept
  {
    firstTable_.swap(other.firstTable_);
    secondTable_.swap(other.secondTable_);
    std::swap(size_, other.size_);
    std::swap(primaryHash_, other.primaryHash_);
    std::swap(secondaryHash_, other.secondaryHash_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::capacity() const noexcept
  {
    return firstTable_.getSize() + secondTable_.getSize();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  bool CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::Iterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::begin() noexcept
  {
    return Iterator(this, 0, 0);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::Iterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::end() noexcept
  {
    return Iterator(this, 2, 0);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::ConstIterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::begin() const noexcept
  {
    return ConstIterator(this, 0, 0);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::ConstIterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::end() const noexcept
  {
    return ConstIterator(this, 2, 0);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::ConstIterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::cbegin() const noexcept
  {
    return begin();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::ConstIterator
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::cend() const noexcept
  {
    return end();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::firstIndex(const Key & key) const
  {
    return primaryHash_(key) % subtableCapacity();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::secondIndex(const Key & key) const
  {
    return secondaryHash_(key) % subtableCapacity();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::subtableCapacity() const noexcept
  {
    return firstTable_.getSize();
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::Node *
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::findNode(const Key & key)
  {
    const size_t firstPosition = firstIndex(key);
    if (firstTable_[firstPosition].occupied_ && equal_(firstTable_[firstPosition].key_, key))
    {
      return std::addressof(firstTable_[firstPosition]);
    }

    const size_t secondPosition = secondIndex(key);
    if (secondTable_[secondPosition].occupied_ && equal_(secondTable_[secondPosition].key_, key))
    {
      return std::addressof(secondTable_[secondPosition]);
    }

    return nullptr;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  const typename CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::Node *
  CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::findNode(const Key & key) const
  {
    const size_t firstPosition = firstIndex(key);
    if (firstTable_[firstPosition].occupied_ && equal_(firstTable_[firstPosition].key_, key))
    {
      return std::addressof(firstTable_[firstPosition]);
    }

    const size_t secondPosition = secondIndex(key);
    if (secondTable_[secondPosition].occupied_ && equal_(secondTable_[secondPosition].key_, key))
    {
      return std::addressof(secondTable_[secondPosition]);
    }

    return nullptr;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::placeWithoutRehash(const Node & node)
  {
    Node current = node;
    size_t tableIndex = 0;
    const size_t maxKickCount = capacity();

    for (size_t kickCount = 0; kickCount < maxKickCount; ++kickCount)
    {
      topit::Vector< Node > & table = tableIndex == 0 ? firstTable_ : secondTable_;
      const size_t position = tableIndex == 0 ? firstIndex(current.key_) : secondIndex(current.key_);

      if (!table[position].occupied_)
      {
        table[position] = current;
        return;
      }

      std::swap(current, table[position]);
      tableIndex = 1 - tableIndex;
    }

    throw std::logic_error("cuckoo cycle");
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::rehashAndPlace(const Node & node)
  {
    size_t newCapacity = capacity();

    while (true)
    {
      try
      {
        placeWithoutRehash(node);
        return;
      }
      catch (const std::logic_error &)
      {
        newCapacity *= maxLoadFactorMultiplier_;
        rehash(newCapacity);
      }
    }
  }
}

#include "cuckoo-hash-iter.hpp"
#include "cuckoo-hash-const-iter.hpp"

#endif
