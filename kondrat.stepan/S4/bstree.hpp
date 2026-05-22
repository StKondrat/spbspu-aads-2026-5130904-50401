#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <stdexcept>
#include "bst-node.hpp"
#include "bst-iter.hpp"
#include "cbst-iter.hpp"

namespace kondrat
{
  template< class Key, class Value, class Compare >
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

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    size_t size() const noexcept;
    bool empty() const noexcept;

    void clear() noexcept;
  private:
    Node< Key, Value > * findNode(const Key & key) const;
    Node< Key, Value > * getMin(Node< Key, Value > * node) const;
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
      if (cmp_(key, current->data_.first))
      {
        current = current->left_;
      }
      else if (cmp_(current->data_.first, key))
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