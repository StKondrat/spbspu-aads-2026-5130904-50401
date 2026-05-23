#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include "bst-node.hpp"
#include "bst-iter.hpp"
#include "cbst-iter.hpp"

namespace kondrat
{
  template< class T >
  struct Less
  {
    bool operator()(const T & lhs, const T & rhs) const
    {
      return lhs < rhs;
    }
  };

  template< class Key, class Value, class Compare = Less< Key > >
  class BSTree
  {
  public:
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = CBSTIterator< Key, Value >;

    BSTree();
    ~BSTree();

    void push(const Key & key, const Value & value);
    Value & get(const Key & key);
    const Value & get(const Key & key) const;
    bool has(const Key & key) const;
    Value drop(const Key & key);
    size_t height() const;
    size_t height(const_iterator it) const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t size() const noexcept;
    bool empty() const noexcept;

    void clear() noexcept;
  private:
    Node< Key, Value > * findNode(const Key & key) const;
    Node< Key, Value > * getMin(Node< Key, Value > * node) const;
    Node< Key, Value > * getMax(Node< Key, Value > * node) const;
    void transplant(Node< Key, Value > * oldNode, Node< Key, Value > * newNode);
    size_t getHeight(Node< Key, Value > * node) const;
    void clear(Node< Key, Value > * node) noexcept;

    Node< Key, Value > * root_;
    size_t size_;
    Compare comp_;
  };

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    root_(nullptr),
    size_(0),
    comp_()
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear();
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(const Key & key, const Value & value)
  {
    Node< Key, Value > * parent = nullptr;
    Node< Key, Value > * current = root_;

    while (current != nullptr)
    {
      parent = current;

      if (comp_(key, current->data_.first))
      {
        current = current->left_;
      }
      else if (comp_(current->data_.first, key))
      {
        current = current->right_;
      }
      else
      {
        current->data_.second = value;
        return;
      }
    }

    Node< Key, Value > * node = new Node< Key, Value >(key, value);
    node->parent_ = parent;

    if (parent == nullptr)
    {
      root_ = node;
    }
    else if (comp_(key, parent->data_.first))
    {
      parent->left_ = node;
    }
    else
    {
      parent->right_ = node;
    }

    ++size_;
  }

  template< class Key, class Value, class Compare >
  Value & BSTree< Key, Value, Compare >::get(const Key & key)
  {
    Node< Key, Value > * node = findNode(key);

    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    return node->data_.second;
  }

  template< class Key, class Value, class Compare >
  const Value & BSTree< Key, Value, Compare >::get(const Key & key) const
  {
    Node< Key, Value > * node = findNode(key);

    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    return node->data_.second;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::has(const Key & key) const
  {
    return findNode(key) != nullptr;
  }

  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::drop(const Key & key)
  {
    Node< Key, Value > * node = findNode(key);

    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    Value value = node->data_.second;

    if (node->left_ == nullptr)
    {
      transplant(node, node->right_);
    }
    else if (node->right_ == nullptr)
    {
      transplant(node, node->left_);
    }
    else
    {
      Node< Key, Value > * next = getMin(node->right_);

      if (next->parent_ != node)
      {
        transplant(next, next->right_);
        next->right_ = node->right_;
        next->right_->parent_ = next;
      }

      transplant(node, next);
      next->left_ = node->left_;
      next->left_->parent_ = next;
    }

    delete node;
    --size_;

    return value;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const
  {
    return getHeight(root_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(const_iterator it) const
  {
    return getHeight(it.node_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::begin()
  {
    return iterator(getMin(root_), root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::end()
  {
    return iterator(nullptr, root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::begin() const
  {
    return const_iterator(getMin(root_), root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::end() const
  {
    return const_iterator(nullptr, root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
  {
    Node< Key, Value > * x = it.node_;

    if (x == nullptr || x->right_ == nullptr)
    {
      throw std::logic_error("invalid rotation");
    }

    Node< Key, Value > * y = x->right_;

    x->right_ = y->left_;

    if (y->left_ != nullptr)
    {
      y->left_->parent_ = x;
    }

    y->parent_ = x->parent_;

    if (x->parent_ == nullptr)
    {
      root_ = y;
    }
    else if (x == x->parent_->left_)
    {
      x->parent_->left_ = y;
    }
    else
    {
      x->parent_->right_ = y;
    }

    y->left_ = x;
    x->parent_ = y;

    return const_iterator(y, root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
  {
    Node< Key, Value > * x = it.node_;

    if (x == nullptr || x->left_ == nullptr)
    {
      throw std::logic_error("invalid rotation");
    }

    Node< Key, Value > * y = x->left_;

    x->left_ = y->right_;

    if (y->right_ != nullptr)
    {
      y->right_->parent_ = x;
    }

    y->parent_ = x->parent_;

    if (x->parent_ == nullptr)
    {
      root_ = y;
    }
    else if (x == x->parent_->left_)
    {
      x->parent_->left_ = y;
    }
    else
    {
      x->parent_->right_ = y;
    }

    y->right_ = x;
    x->parent_ = y;

    return const_iterator(y, root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
  {
    Node< Key, Value > * x = it.node_;

    if (x == nullptr || x->right_ == nullptr)
    {
      throw std::logic_error("invalid rotation");
    }

    rotateRight(const_iterator(x->right_, root_));
    return rotateLeft(const_iterator(x, root_));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
  {
    Node< Key, Value > * x = it.node_;

    if (x == nullptr || x->left_ == nullptr)
    {
      throw std::logic_error("invalid rotation");
    }

    rotateLeft(const_iterator(x->left_, root_));
    return rotateRight(const_iterator(x, root_));
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear() noexcept
  {
    clear(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  Node< Key, Value > * BSTree< Key, Value, Compare >::findNode(const Key & key) const
  {
    Node< Key, Value > * current = root_;

    while (current != nullptr)
    {
      if (comp_(key, current->data_.first))
      {
        current = current->left_;
      }
      else if (comp_(current->data_.first, key))
      {
        current = current->right_;
      }
      else
      {
        return current;
      }
    }

    return nullptr;
  }

  template< class Key, class Value, class Compare >
  Node< Key, Value > * BSTree< Key, Value, Compare >::getMin(Node< Key, Value > * node) const
  {
    if (node == nullptr)
    {
      return nullptr;
    }

    while (node->left_ != nullptr)
    {
      node = node->left_;
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  Node< Key, Value > * BSTree< Key, Value, Compare >::getMax(Node< Key, Value > * node) const
  {
    if (node == nullptr)
    {
      return nullptr;
    }

    while (node->right_ != nullptr)
    {
      node = node->right_;
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::transplant(Node< Key, Value > * oldNode, Node< Key, Value > * newNode)
  {
    if (oldNode->parent_ == nullptr)
    {
      root_ = newNode;
    }
    else if (oldNode == oldNode->parent_->left_)
    {
      oldNode->parent_->left_ = newNode;
    }
    else
    {
      oldNode->parent_->right_ = newNode;
    }

    if (newNode != nullptr)
    {
      newNode->parent_ = oldNode->parent_;
    }
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::getHeight(Node< Key, Value > * node) const
  {
    if (node == nullptr)
    {
      return 0;
    }

    size_t leftHeight = getHeight(node->left_);
    size_t rightHeight = getHeight(node->right_);

    return std::max(leftHeight, rightHeight) + 1;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear(Node< Key, Value > * node) noexcept
  {
    if (node == nullptr)
    {
      return;
    }

    clear(node->left_);
    clear(node->right_);
    delete node;
  }
}

#endif
