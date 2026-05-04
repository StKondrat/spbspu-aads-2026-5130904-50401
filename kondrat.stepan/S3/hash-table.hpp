#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include "hash-node.hpp"

namespace kondrat
{
  template < class Key, class Value, class Hash, class Equal >
  struct HashTable
  {
    HashTable();
    HashTable(size_t capacity);
    ~HashTable();
    HashTable(const HashTable & table);
    HashTable & operator=(const HashTable & table);
    HashTable(HashTable && table);
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
    HashNode< Key, Value > * data_;
    size_t size_;
    size_t capacity_;
    Hash hash_;
    Equal equal_;
  };
}

#endif