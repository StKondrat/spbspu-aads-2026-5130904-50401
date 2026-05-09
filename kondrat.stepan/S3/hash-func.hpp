#ifndef HASH_FUNC_HPP
#define HASH_FUNC_HPP

#include <cstddef>
#include <string>
#include <boost/hash2/blake2.hpp>

namespace kondrat
{
  struct blake2
  {
    size_t operator()(const std::string & str) const
    {
      boost::hash2::blake2b_512 hash;
      hash.update(str.data(), str.size());

      boost::hash2::blake2b_512::result_type digest = hash.result();

      size_t result = 0;
      const unsigned char * bytes = digest.data();

      for (size_t i = 0; i < sizeof(size_t); ++i)
      {
        result = result * 256 + bytes[i];
      }

      return result;
    }
  };
}

#endif