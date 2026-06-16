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
      static const size_t maxRehashCount_ = 32;

      topit::Vector< Node > firstTable_;
      topit::Vector< Node > secondTable_;
      size_t size_;
      PrimHash primaryHash_;
      SecHash secondaryHash_;
      Equal equal_;

      size_t firstIndex(const Key & key) const;
      size_t secondIndex(const Key & key) const;
      size_t subtableCapacity() const noexcept;
      size_t normalizedSubtableCapacity(size_t capacity) const;

      Node * findNode(const Key & key);
      const Node * findNode(const Key & key) const;

      bool placeWithoutRehash(const Node & node);
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
    firstTable_(normalizedSubtableCapacity(capacity), Node()),
    secondTable_(normalizedSubtableCapacity(capacity), Node()),
    size_(0),
    primaryHash_(),
    secondaryHash_(),
    equal_()
  {}

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::add(const Key & key, const Value & value)
  {
    CuckooHashTable copy(*this);

    Node * existing = copy.findNode(key);
    if (existing != nullptr)
    {
      existing->value_ = value;
    }
    else
    {
      if ((copy.size_ + 1) * maxLoadFactorMultiplier_ > copy.capacity())
      {
        copy.rehash(copy.capacity() * maxLoadFactorMultiplier_);
      }

      copy.rehashAndPlace(Node(key, value));
      ++copy.size_;
    }

    swap(copy);
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  Value CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::drop(const Key & key)
  {
    CuckooHashTable copy(*this);
    Node * node = copy.findNode(key);
    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    Value value = node->value_;
    *node = Node();
    --copy.size_;

    swap(copy);
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
    CuckooHashTable copy(*this);

    for (size_t i = 0; i < copy.firstTable_.getSize(); ++i)
    {
      copy.firstTable_[i] = Node();
      copy.secondTable_[i] = Node();
    }
    copy.size_ = 0;

    swap(copy);
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
  size_t CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::normalizedSubtableCapacity(size_t capacity) const
  {
    if (capacity == 0)
    {
      throw std::logic_error("invalid capacity");
    }

    const size_t halfCapacity = (capacity + 1) / 2;

    return halfCapacity < minSubtableCapacity_ ? minSubtableCapacity_ : halfCapacity;
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
  bool CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::placeWithoutRehash(const Node & node)
  {
    topit::Vector< Node > firstBackup = firstTable_;
    topit::Vector< Node > secondBackup = secondTable_;
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
        return true;
      }

      std::swap(current, table[position]);
      tableIndex = 1 - tableIndex;
    }

    firstTable_.swap(firstBackup);
    secondTable_.swap(secondBackup);
    return false;
  }

  template< class Key, class Value, class PrimHash, class SecHash, class Equal >
  void CuckooHashTable< Key, Value, PrimHash, SecHash, Equal >::rehashAndPlace(const Node & node)
  {
    size_t newCapacity = capacity();

    for (size_t rehashCount = 0; rehashCount < maxRehashCount_; ++rehashCount)
    {
      if (placeWithoutRehash(node))
      {
        return;
      }

      newCapacity *= maxLoadFactorMultiplier_;
      rehash(newCapacity);
    }

    throw std::logic_error("rehash limit exceeded");
  }
}

#include "cuckoo-hash-iter.hpp"
#include "cuckoo-hash-const-iter.hpp"

#endif
