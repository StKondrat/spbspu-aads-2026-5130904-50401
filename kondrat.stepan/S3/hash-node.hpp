#ifndef HASH_NODE_HPP
#define HASH_NODE_HPP

namespace kondrat
{
  enum HashNodeState
  {
    EMPTY,
    OCCUPIED,
    TOMBSTONE
  };

  template< class Key, class Value >
  struct HashNode
  {
    Key key;
    Value value;
    HashNodeState state;

    HashNode();
    HashNode(const Key & k, const Value & v, HashNodeState st);
  };

  template< class Key, class Value >
  HashNode< Key, Value >::HashNode():
    key(),
    value(),
    state(EMPTY)
  {}

  template< class Key, class Value >
  HashNode< Key, Value >::HashNode(const Key & k, const Value & v, HashNodeState st):
    key(k),
    value(v),
    state(st)
  {}
}

#endif
