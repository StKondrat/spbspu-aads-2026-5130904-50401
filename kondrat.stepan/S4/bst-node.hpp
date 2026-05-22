#ifndef BST_NODE_HPP
#define BST_NODE_HPP

#include <utility>

namespace kondrat
{
  template< class Key, class Value >
  class Node
  {
  public:
    Node(const Key & key, const Value & value);
  private:
    template< class K, class V >
    friend class BSTIterator;
    template< class K, class V >
    friend class CBSTIterator;
    template< class K, class V, class C >
    friend class BSTree;

    std::pair< Key, Value > data_;
    Node * right_;
    Node * left_;
    Node * parent_;
  };
}

template< class Key, class Value >
kondrat::Node< Key, Value >::Node(const Key & key, const Value & value):
  data_(std::pair< Key, Value >(key, value)),
  right_(nullptr),
  left_(nullptr),
  parent_(nullptr)
{}

#endif
