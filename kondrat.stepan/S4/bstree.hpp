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
}

#endif