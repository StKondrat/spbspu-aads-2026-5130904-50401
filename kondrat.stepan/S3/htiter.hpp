#ifndef HTITER_HPP
#define HTITER_HPP

#include <cassert>
#include <memory>
#include "hash-node.hpp"

namespace kondrat
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable;

  template< class Key, class Value >
  struct HTIter
  {
    template< class K, class V, class H, class E >
    friend struct HashTable;

    HTIter();

    HTIter< Key, Value > & operator++();
    HTIter< Key, Value > operator++(int);

    bool operator==(const HTIter< Key, Value > & rhs) const;
    bool operator!=(const HTIter< Key, Value > & rhs) const;

    HashNode< Key, Value > & operator*() const;
    HashNode< Key, Value > * operator->() const;

  private:
    HTIter(HashNode< Key, Value > * node, HashNode< Key, Value > * end);
    void skipInvalid();

    HashNode< Key, Value > * node_;
    HashNode< Key, Value > * end_;
  };

  template< class Key, class Value >
  HTIter< Key, Value >::HTIter():
    node_(nullptr),
    end_(nullptr)
  {}

  template< class Key, class Value >
  HTIter< Key, Value > & HTIter< Key, Value >::operator++()
  {
    assert(node_ != nullptr);

    if (node_ != end_)
    {
      ++node_;
      skipInvalid();
    }

    return *this;
  }

  template< class Key, class Value >
  HTIter< Key, Value > HTIter< Key, Value >::operator++(int)
  {
    HTIter< Key, Value > temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value >
  bool HTIter< Key, Value >::operator==(const HTIter< Key, Value > & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool HTIter< Key, Value >::operator!=(const HTIter< Key, Value > & rhs) const
  {
    return node_ != rhs.node_;
  }

  template< class Key, class Value >
  HashNode< Key, Value > & HTIter< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return *node_;
  }

  template< class Key, class Value >
  HashNode< Key, Value > * HTIter< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return std::addressof(*node_);
  }

  template< class Key, class Value >
  HTIter< Key, Value >::HTIter(HashNode< Key, Value > * node, HashNode< Key, Value > * end):
    node_(node),
    end_(end)
  {
    skipInvalid();
  }

  template< class Key, class Value >
  void HTIter< Key, Value >::skipInvalid()
  {
    while (node_ != end_ && node_->state != OCCUPIED)
    {
      ++node_;
    }
  }
}

#endif
