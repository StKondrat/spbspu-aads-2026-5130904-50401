#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include "hash-node.hpp"

namespace kondrat
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable
  {
    HashTable();
    HashTable(size_t capacity);
    ~HashTable();
    HashTable(const HashTable & table);
    HashTable(HashTable && table);
    HashTable & operator=(const HashTable & table);
    HashTable & operator=(HashTable && table);

    void add(const Key & key, const Value & value);
    Value drop(const Key & key);

    bool has(const Key & key) const;
    Value & get(const Key & key);
    const Value & get(const Key & key) const;

    void rehash(size_t capacity);
    void clear();
    void swap(HashTable & table);

    size_t size() const;
    size_t capacity() const;
    bool empty() const;

  private:
    size_t getIndex(const Key & key, size_t attempt) const;
    size_t findIndex(const Key & key) const;
    size_t findSlot(const Key & key) const;

    HashNode< Key, Value > * data_;
    size_t size_;
    size_t capacity_;
    Hash hash_;
    Equal equal_;
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    data_(new HashNode< Key, Value >[16]),
    size_(0),
    capacity_(16),
    hash_(),
    equal_()
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t capacity):
    data_(nullptr),
    size_(0),
    capacity_(capacity),
    hash_(),
    equal_()
  {
    if (capacity_ == 0)
    {
      throw std::logic_error("invalid capacity");
    }
    data_ = new HashNode< Key, Value >[capacity_];
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable()
  {
    delete[] data_;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable & table):
    data_(new HashNode< Key, Value >[table.capacity_]),
    size_(table.size_),
    capacity_(table.capacity_),
    hash_(table.hash_),
    equal_(table.equal_)
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      data_[i] = table.data_[i];
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable && table):
    data_(table.data_),
    size_(table.size_),
    capacity_(table.capacity_),
    hash_(table.hash_),
    equal_(table.equal_)
  {
    table.data_ = nullptr;
    table.size_ = 0;
    table.capacity_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal > & HashTable< Key, Value, Hash, Equal >::operator=(const HashTable & table)
  {
    if (this == std::addrassof(table))
    {
      return *this;
    }
    HashTable< Key, Value, Hash, Equal > cpy = table;
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal > & HashTable< Key, Value, Hash, Equal >::operator=(HashTable && table)
  {
    if (this == std::addressof(table))
    {
      return *this;
    }
    HashTable< Key, Value, Hash, Equal > cpy(std::move(table));
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key & key, const Value & value)
  {
    size_t index = findSlot(key);

    if (data_[index].state == OCCUPIED)
    {
      data_[index].value = value;
    }
    else
    {
      data_[index].key = key;
      data_[index].value = value;
      data_[index].state = OCCUPIED;
      ++size_;
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(const Key & key)
  {
    size_t index = findIndex(key);

    if (index == capacity_)
    {
      throw std::logic_error("key not found");
    }

    Value cpy = data_[index].value;
    data_[index].state = TOMBSTONE;
    --size_;

    return cpy;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::getIndex(const Key & key, size_t attempt) const
  {
    return (hash_(key) + attempt * attempt) % capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findIndex(const Key & key) const
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t index = getIndex(key, i);

      if (data_[index].state == EMPTY)
      {
        return capacity_;
      }

      if (data_[index].state == OCCUPIED && equal_(data_[index].key, key))
      {
        return index;
      }
    }

    return capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findSlot(const Key & key) const
  {
    size_t firstTombstone = capacity_;

    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t index = getIndex(key, i);

      if (data_[index].state == OCCUPIED)
      {
        if (equal_(data_[index].key, key))
        {
          return index;
        }
      }
      else if (data_[index].state == TOMBSTONE)
      {
        if (firstTombstone == capacity_)
        {
          firstTombstone = index;
        }
      }
      else
      {
        if (firstTombstone != capacity_)
        {
          return firstTombstone;
        }
        return index;
      }
    }

    if (firstTombstone != capacity_)
    {
      return firstTombstone;
    }

    throw std::overflow_error("hash table is full");
  }
}

#endif