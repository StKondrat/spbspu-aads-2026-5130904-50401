#include "graph.hpp"
#include <stdexcept>

namespace kondrat
{
  Graph::Graph():
    vertexes_(),
    edges_()
  {}

  void Graph::addVertex(const std::string & vertex)
  {
    vertexes_.add(vertex, true);
  }

  bool Graph::hasVertex(const std::string & vertex) const
  {
    return vertexes_.has(vertex);
  }

  void Graph::bind(const std::string & from, const std::string & to, size_t weight)
  {
    Graph cpy(*this);

    cpy.vertexes_.add(from, true);
    cpy.vertexes_.add(to, true);

    EdgeKey key(from, to);

    if (cpy.edges_.has(key))
    {
      cpy.edges_.get(key).pushBack(weight);
    }
    else
    {
      WeightList weights;
      weights.pushBack(weight);
      cpy.edges_.add(key, weights);
    }

    swap(cpy);
  }

  void Graph::cut(const std::string & from, const std::string & to, size_t weight)
  {
    if (!hasVertex(from) || !hasVertex(to))
    {
      throw std::logic_error("invalid graph operation");
    }

    EdgeKey key(from, to);

    if (!edges_.has(key))
    {
      throw std::logic_error("invalid graph operation");
    }

    Graph cpy(*this);
    WeightList & weights = cpy.edges_.get(key);

    bool found = false;

    for (size_t i = 0; i < weights.getSize(); ++i)
    {
      if (weights[i] == weight)
      {
        weights.erase(i);
        found = true;
        break;
      }
    }

    if (!found)
    {
      throw std::logic_error("invalid graph operation");
    }

    if (weights.isEmpty())
    {
      cpy.edges_.drop(key);
    }

    swap(cpy);
  }

  topit::Vector< std::string > Graph::getVertexes() const
  {
    topit::Vector< std::string > result;

    HTCIter< std::string, bool > it = vertexes_.begin();
    HTCIter< std::string, bool > end = vertexes_.end();

    while (it != end)
    {
      result.pushBack(it->key);
      ++it;
    }

    return result;
  }

  topit::Vector< EdgeInfo > Graph::getOutbound(const std::string & vertex) const
  {
    if (!hasVertex(vertex))
    {
      throw std::logic_error("invalid graph operation");
    }

    topit::Vector< EdgeInfo > result;

    HTCIter< EdgeKey, WeightList > it = edges_.begin();
    HTCIter< EdgeKey, WeightList > end = edges_.end();

    while (it != end)
    {
      if (it->key.first == vertex)
      {
        EdgeInfo info(it->key.second, it->value);
      }
      ++it;
    }

    return result;
  }

  topit::Vector< EdgeInfo > Graph::getInbound(const std::string & vertex) const
  {
    if (!hasVertex(vertex))
    {
      throw std::logic_error("invalid graph operator");
    }

    topit::Vector< EdgeInfo > result;

    HTCIter< EdgeKey, WeightList > it = edges_.begin();
    HTCIter< EdgeKey, WeightList > end = edges_.end();

    while (it != end)
    {
      if (it->key.second == vertex)
      {
        EdgeInfo info(it->key.first, it->value);
        result.pushBack(info);
      }
      ++it;
    }

    return result;
  }

  void Graph::mergeFrom(const Graph & first, const Graph & second)
  {
    Graph cpy;

    topit::Vector< std::string > firstVertexes = first.getVertexes();
    topit::Vector< std::string > secondVertexes = second.getVertexes();

    for (size_t i = 0; i < firstVertexes.getSize(); ++i)
    {
      cpy.vertexes_.add(firstVertexes[i], true);
    }

    for (size_t i = 0; i < secondVertexes.getSize(); ++i)
    {
      cpy.vertexes_.add(secondVertexes[i], true);
    }

    HTCIter< EdgeKey, WeightList > it1 = first.edges_.begin();
    HTCIter< EdgeKey, WeightList > end1 = first.edges_.end();

    while (it1 != end1)
    {
      for (size_t i = 0; i < it1->value.getSize(); ++i)
      {
        cpy.bind(it1->key.first, it1->key.second, it1->value[i]);
      }
      ++it1;
    }

    HTCIter< EdgeKey, WeightList > it2 = second.edges_.begin();
    HTCIter< EdgeKey, WeightList > end2 = second.edges_.end();

    while (it2 != end2)
    {
      for (size_t i = 0; i < it2->value.getSize(); ++i)
      {
        cpy.bind(it2->key.first, it2->key.second, it2->value[i]);
      }
      ++it2;
    }

    swap(cpy);
  }

  void Graph::extractFrom(const Graph & graph, const topit::Vector< std::string > & vertexes)
  {
    Graph cpy;

    for (size_t i = 0; i < vertexes.getSize(); ++i)
    {
      if (!graph.hasVertex(vertexes[i]))
      {
        throw std::logic_error("invalid graph operation");
      }

      cpy.vertexes_.add(vertexes[i], true);
    }

    HTCIter< EdgeKey, WeightList > it = graph.edges_.begin();
    HTCIter< EdgeKey, WeightList > end = graph.edges_.end();

    while (it != end)
    {
      if (cpy.hasVertex(it->key.first) && cpy.hasVertex(it->key.second))
      {
        for (size_t i = 0; i < it->value.getSize(); ++i)
        {
          cpy.bind(it->key.first, it->key.second, it->value[i]);
        }
      }
      ++it;
    }

    swap(cpy);
  }

  void Graph::swap(Graph & graph) noexcept
  {
    vertexes_.swap(graph.vertexes_);
    edges_.swap(graph.edges_);
  }
}
