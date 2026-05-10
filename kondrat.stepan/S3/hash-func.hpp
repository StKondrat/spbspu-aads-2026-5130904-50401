#ifndef HASH_FUNC_HPP
#define HASH_FUNC_HPP

#include <cstddef>
#include <string>
#include <boost/hash2/blake2.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace kondrat
{
  struct blake2
  {
    size_t operator()(const std::string & str) const
    {
      boost::hash2::blake2b_512 hash;
      hash.update(str.data(), str.size());

      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };
}

#endif