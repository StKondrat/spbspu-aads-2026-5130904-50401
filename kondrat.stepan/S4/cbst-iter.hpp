#ifndef CBST_ITER_HPP
#define CBST_ITER_HPP

#include <cassert>
#include "bst-node.hpp"

namespace kondrat
{
  template< class Key, class Value >
  class CBSTIterator
  {
  public:
    CBSTIterator();

    CBSTIterator & operator++();
    CBSTIterator operator++(int);
    CBSTIterator & operator--();
    CBSTIterator operator--(int);

    bool operator==(const CBSTIterator & rhs) const;
    bool operator!=(const CBSTIterator & rhs) const;

    const std::pair< Key, Value > & operator*() const;
    const std::pair< Key, Value > * operator->() const;

  private:
    template< class K, class V, class C >
    friend class BSTree;
    CBSTIterator(Node< Key, Value > * node, Node< Key, Value > * root);
    Node< Key, Value > * getMin(Node< Key, Value > * node) const;
    Node< Key, Value > * getMax(Node< Key, Value > * node) const;

    Node< Key, Value > * node_;
    Node< Key, Value > * root_;
  };

  template< class Key, class Value >
  CBSTIterator< Key, Value >::CBSTIterator():
    node_(nullptr),
    root_(nullptr)
  {}

  template< class Key, class Value >
  CBSTIterator< Key, Value > & CBSTIterator< Key, Value >::operator++()
  {
    assert(node_ != nullptr);

    if (node_->right_)
    {
      node_ = getMin(node_->right_);
    }
    else
    {
      Node< Key, Value > * parent = node_->parent_;
      while (parent && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
    }

    return *this;
  }

  template< class Key, class Value >
  CBSTIterator< Key, Value > CBSTIterator< Key, Value >::operator++(int)
  {
    BSTIterator temp(*this);
    ++(*this);
    return temp;
  }

  template< class Key, class Value >
  CBSTIterator< Key, Value > & CBSTIterator< Key, Value >::operator--()
  {
    if (node_ == nullptr)
    {
      node_ = getMax(root_);
      return *this;
    }

    if (node_->left_)
    {
      node_ = getMax(node_->left_);
    }
    else
    {
      Node< Key, Value > * parent = node_->parent_;
      while (parent && node_ == parent->left_)
      {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
    }

    return *this;
  }

  template< class Key, class Value >
  CBSTIterator< Key, Value > CBSTIterator< Key, Value >::operator--(int)
  {
    BSTIterator temp(*this);
    --(*this);
    return temp;
  }

  template< class Key, class Value >
  bool CBSTIterator< Key, Value >::operator==(const CBSTIterator & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool CBSTIterator< Key, Value >::operator!=(const CBSTIterator & rhs) const
  {
    return node_ != rhs.node_;
  }

  template< class Key, class Value >
  const std::pair< Key, Value > & CBSTIterator< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< class Key, class Value >
  const std::pair< Key, Value > * CBSTIterator< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< class Key, class Value >
  CBSTIterator< Key, Value >::CBSTIterator(Node< Key, Value > * node, Node< Key, Value > * root):
    node_(node),
    root_(root)
  {}

  template< class Key, class Value >
  Node< Key, Value > * CBSTIterator< Key, Value >::getMin(Node< Key, Value > * node) const
  {
    while (node && node->left_)
    {
      node = node->left_;
    }

    return node;
  }

  template< class Key, class Value >
  Node< Key, Value > * CBSTIterator< Key, Value >::getMax(Node< Key, Value > * node) const
  {
    while (node && node->right_)
    {
      node = node->right_;
    }

    return node;
  }
}

#endif