#ifndef HASH_FUNC_HPP
#define HASH_FUNC_HPP

#include <cstddef>
#include <string>
#include <boost/hash2/blake2.hpp>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace kondrat
{
  struct Hash1
  {
    size_t operator()(const std::string & value) const
    {
      boost::hash2::blake2b_512 hash;
      hash.update(value.data(), value.size());

      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };

  struct Hash2
  {
    size_t operator()(const std::string & value) const
    {
      boost::hash2::xxhash_64 hash;
      hash.update(value.data(), value.size());

      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };
}

#endif
