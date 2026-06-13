#ifndef CUCKOO_HASH_NODE_HPP
#define CUCKOO_HASH_NODE_HPP

namespace kondrat
{
  template< class Key, class Value >
  struct CuckooHashNode
  {
    Key key;
    Value value;
    bool occupied;

    CuckooHashNode();
    CuckooHashNode(const Key & k, const Value & v);
  };

  template< class Key, class Value >
  CuckooHashNode< Key, Value >::CuckooHashNode():
    key(),
    value(),
    occupied(false)
  {}

  template< class Key, class Value >
  CuckooHashNode< Key, Value >::CuckooHashNode(const Key & k, const Value & v):
    key(k),
    value(v),
    occupied(true)
  {}
}

#endif
