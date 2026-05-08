#ifndef HTCITER_HPP
#define HTCITER_HPP

#include <cassert>
#include <memory>
#include "hash-node.hpp"

namespace kondrat
{
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable;

  template< class Key, class Value >
  struct HTCIter
  {
    template< class K, class V, class H, class E >
    friend struct HashTable;

    HTCIter();

    HTCIter< Key, Value > & operator++();
    HTCIter< Key, Value > operator++(int);

    bool operator==(const HTCIter< Key, Value > & rhs) const;
    bool operator!=(const HTCIter< Key, Value > & rhs) const;

    const HashNode< Key, Value > & operator*() const;
    const HashNode< Key, Value > * operator->() const;

  private:
    HTCIter(const HashNode< Key, Value > * node, const HashNode< Key, Value > * end);
    void skipInvalid();

    const HashNode< Key, Value > * node_;
    const HashNode< Key, Value > * end_;
  };

  template< class Key, class Value >
  HTCIter< Key, Value >::HTCIter():
    node_(nullptr),
    end_(nullptr)
  {}

  template< class Key, class Value >
  HTCIter< Key, Value >::HTCIter(const HashNode< Key, Value > * node, const HashNode< Key, Value > * end):
    node_(node),
    end_(end)
  {
    skipInvalid();
  }

  template< class Key, class Value >
  HTCIter< Key, Value > & HTCIter< Key, Value >::operator++()
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
  HTCIter< Key, Value > HTCIter< Key, Value >::operator++(int)
  {
    HTCIter< Key, Value > temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value >
  bool HTCIter< Key, Value >::operator==(const HTCIter< Key, Value > & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool HTCIter< Key, Value >::operator!=(const HTCIter< Key, Value > & rhs) const
  {
    return node_ != rhs.node_;
  }

  template< class Key, class Value >
  const HashNode< Key, Value > & HTCIter< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return *node_;
  }

  template< class Key, class Value >
  const HashNode< Key, Value > * HTCIter< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    assert(node_ != end_);
    return std::addressof(*node_);
  }

  template< class Key, class Value >
  void HTCIter< Key, Value >::skipInvalid()
  {
    while (node_ != end_ && node_->state != OCCUPIED)
    {
      ++node_;
    }
  }
}

#endif