#ifndef BST_ITER_HPP
#define BST_ITER_HPP

#include <cassert>
#include "bst-node.hpp"

namespace kondrat
{
  template< class Key, class Value >
  class BSTIterator
  {
  public:
    BSTIterator();

    BSTIterator & operator++();
    BSTIterator operator++(int);
    BSTIterator & operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator & rhs) const;
    bool operator!=(const BSTIterator & rhs) const;

    std::pair< Key, Value > & operator*() const;
    std::pair< Key, Value > * operator->() const;

  private:
    template< class K, class V, class C >
    friend class BSTree;
    BSTIterator(Node< Key, Value > * node, Node< Key, Value > * root);
    Node< Key, Value > * getMin(Node< Key, Value > * node) const;
    Node< Key, Value > * getMax(Node< Key, Value > * node) const;

    Node< Key, Value > * node_;
    Node< Key, Value > * root_;
  };

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator():
    node_(nullptr),
    root_(nullptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value > & BSTIterator< Key, Value >::operator++()
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
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator++(int)
  {
    BSTIterator temp(*this);
    ++(*this);
    return temp;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > & BSTIterator< Key, Value >::operator--()
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
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator--(int)
  {
    BSTIterator temp(*this);
    --(*this);
    return temp;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator & rhs) const
  {
    return node_ != rhs.node_;
  }

  template< class Key, class Value >
  std::pair< Key, Value > & BSTIterator< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< class Key, class Value >
  std::pair< Key, Value > * BSTIterator< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(Node< Key, Value > * node, Node< Key, Value > * root):
    node_(node),
    root_(root)
  {}

  template< class Key, class Value >
  Node< Key, Value > * BSTIterator< Key, Value >::getMin(Node< Key, Value > * node) const
  {
    while (node && node->left_)
    {
      node = node->left_;
    }

    return node;
  }

  template< class Key, class Value >
  Node< Key, Value > * BSTIterator< Key, Value >::getMax(Node< Key, Value > * node) const
  {
    while (node && node->right_)
    {
      node = node->right_;
    }

    return node;
  }
}
#endif
