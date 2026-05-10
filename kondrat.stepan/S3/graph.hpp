#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <cstddef>
#include <utility>
#include "hash-table.hpp"
#include "hash-func.hpp"
#include "../Common/top-it-vector.hpp"

namespace kondrat
{
  using WeightList = topit::Vector< size_t >;
  using EdgeInfo = std::pair< std::string, WeightList >;

  struct Graph
  {
    Graph();

    void addVertex(const std::string & vertex);
    bool hasVertex(const std::string & vertex) const;

    void bind(const std::string & from, const std::string & to, size_t weight);
    void cut(const std::string & from, const std::string & to, size_t weight);

    topit::Vector< std::string > getVertexes() const;
    topit::Vector< EdgeInfo > getOutbound(const std::string & vertex) const;
    topit::Vector< EdgeInfo > getInbound(const std::string & vertex) const;

    void mergeFrom(const Graph & first, const Graph & second);
    void extractFrom(const Graph & graph, const topit::Vector< std::string > & vertexes);

  private:
    HashTable< std::string, bool, blake2, Equal< std::string > > vertexes_;
    HashTable< EdgeKey, WeightList, edgeBlake2, Equal< EdgeKey > > edges_;
  };
}

#endif