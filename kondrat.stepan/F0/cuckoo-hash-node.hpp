#ifndef CUCKOO_HASH_NODE_HPP
#define CUCKOO_HASH_NODE_HPP

namespace kondrat
{
  template< class Key, class Value >
  struct CuckooHashNode
  {
    Key key_;
    Value value_;
    bool occupied_;

    CuckooHashNode();
    CuckooHashNode(const Key & k, const Value & v);
  };

  template< class Key, class Value >
  CuckooHashNode< Key, Value >::CuckooHashNode():
    key_(),
    value_(),
    occupied_(false)
  {}

  template< class Key, class Value >
  CuckooHashNode< Key, Value >::CuckooHashNode(const Key & k, const Value & v):
    key_(k),
    value_(v),
    occupied_(true)
  {}
}

#endif
