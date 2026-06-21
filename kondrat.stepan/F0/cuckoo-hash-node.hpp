#ifndef CUCKOO_HASH_NODE_HPP
#define CUCKOO_HASH_NODE_HPP

namespace kondrat
{
  namespace detail
  {
    template< class Key, class Value >
    struct CuckooHashNode
    {
      Key key_;
      Value value_;
      bool occupied_;

      CuckooHashNode();
      CuckooHashNode(const Key & key, const Value & value);
    };

    template< class Key, class Value >
    CuckooHashNode< Key, Value >::CuckooHashNode():
      key_(),
      value_(),
      occupied_(false)
    {}

    template< class Key, class Value >
    CuckooHashNode< Key, Value >::CuckooHashNode(const Key & key, const Value & value):
      key_(key),
      value_(value),
      occupied_(true)
    {}
  }
}

#endif
