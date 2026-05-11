#include <istream>
#include <ostream>
#include <stdexcept>
#include "graphs-table.hpp"

namespace kondrat
{
  using EdgeList = topit::Vector< EdgeInfo >;

  template< class T >
  void sortVector(topit::Vector< T > & values)
  {
    for (size_t i = 0; i < values.getSize(); ++i)
    {
      for (size_t j = i + 1; j < values.getSize(); ++j)
      {
        if (values[j] < values[i])
        {
          T tmp = values[i];
          values[i] = values[j];
          values[j] = tmp;
        }
      }
    }
  }

  void sortEdges(EdgeList & edges)
  {
    for (size_t i = 0; i < edges.getSize(); ++i)
    {
      for (size_t j = i + 1; j < edges.getSize(); ++j)
      {
        if (edges[j].first < edges[i].first)
        {
          EdgeInfo tmp = edges[i];
          edges[i] = edges[j];
          edges[j] = tmp;
        }
      }
    }
  }

  void printEdges(std::ostream & out, EdgeList & edges)
  {
    sortEdges(edges);

    for (size_t i = 0; i < edges.getSize(); ++i)
    {
      sortVector(edges[i].second);

      out << edges[i].first;
      for (size_t j = 0; j < edges[i].second.getSize(); ++j)
      {
        out << ' ' << edges[i].second[j];
      }
      out << '\n';
    }
  }

  void GraphsTable::readFile(std::istream & in)
  {
    std::string graphName;
    size_t edgesCount = 0;

    while (in >> graphName >> edgesCount)
    {
      Graph graph;

      for (size_t i = 0; i < edgesCount; ++i)
      {
        std::string from;
        std::string to;
        size_t weight = 0;

        if (!(in >> from >> to >> weight))
        {
          throw std::logic_error("invalid file");
        }

        graph.bind(from, to, weight);
      }

      graphs_.add(graphName, graph);
    }
  }

  void GraphsTable::graphs(std::istream &, std::ostream & out, std::string)
  {
    topit::Vector< std::string > names;

    HTIter< std::string, Graph > it = graphs_.begin();
    HTIter< std::string, Graph > end = graphs_.end();

    while (it != end)
    {
      names.pushBack(it->key);
      ++it;
    }

    sortVector(names);

    for (size_t i = 0; i < names.getSize(); ++i)
    {
      out << names[i] << '\n';
    }
  }

  void GraphsTable::vertexes(std::istream &, std::ostream & out, std::string graphName)
  {
    Graph & graph = graphs_.get(graphName);
    topit::Vector< std::string > vertexes = graph.getVertexes();

    sortVector(vertexes);

    for (size_t i = 0; i < vertexes.getSize(); ++i)
    {
      out << vertexes[i] << '\n';
    }
  }

  void GraphsTable::outbound(std::istream & in, std::ostream & out, std::string graphName)
  {
    std::string vertex;

    if (!(in >> vertex))
    {
      throw std::logic_error("invalid command");
    }

    Graph & graph = graphs_.get(graphName);
    EdgeList edges = graph.getOutbound(vertex);

    printEdges(out, edges);
  }

  void GraphsTable::inbound(std::istream & in, std::ostream & out, std::string graphName)
  {
    std::string vertex;

    if (!(in >> vertex))
    {
      throw std::logic_error("invalid command");
    }

    Graph & graph = graphs_.get(graphName);
    EdgeList edges = graph.getInbound(vertex);

    printEdges(out, edges);
  }

  void GraphsTable::bind(std::istream & in, std::ostream &, std::string graphName)
  {
    std::string from;
    std::string to;
    size_t weight = 0;

    if (!(in >> from >> to >> weight))
    {
      throw std::logic_error("invalid command");
    }

    Graph & graph = graphs_.get(graphName);
    graph.bind(from, to, weight);
  }

  void GraphsTable::cut(std::istream & in, std::ostream &, std::string graphName)
  {
    std::string from;
    std::string to;
    size_t weight = 0;

    if (!(in >> from >> to >> weight))
    {
      throw std::logic_error("invalid command");
    }

    Graph & graph = graphs_.get(graphName);
    graph.cut(from, to, weight);
  }

  void GraphsTable::create(std::istream &, std::ostream &, std::string graphName)
  {
    if (graphs_.has(graphName))
    {
      throw std::logic_error("invalid command");
    }

    Graph graph;

    GraphStorage copy(graphs_);
    copy.add(graphName, graph);
    graphs_.swap(copy);
  }

  void GraphsTable::merge(std::istream & in, std::ostream &, std::string graphName)
  {
    std::string firstName;
    std::string secondName;

    if (!(in >> firstName >> secondName))
    {
      throw std::logic_error("invalid command");
    }

    if (graphs_.has(graphName) || !graphs_.has(firstName) || !graphs_.has(secondName))
    {
      throw std::logic_error("invalid command");
    }

    Graph graph;
    graph.mergeFrom(graphs_.get(firstName), graphs_.get(secondName));

    GraphStorage copy(graphs_);
    copy.add(graphName, graph);
    graphs_.swap(copy);
  }

  void GraphsTable::extract(std::istream & in, std::ostream &, std::string graphName)
  {
    std::string oldName;
    size_t count = 0;

    if (!(in >> oldName >> count))
    {
      throw std::logic_error("invalid command");
    }

    if (graphs_.has(graphName) || !graphs_.has(oldName))
    {
      throw std::logic_error("invalid command");
    }

    topit::Vector< std::string > vertexes;

    for (size_t i = 0; i < count; ++i)
    {
      std::string vertex;
      if (!(in >> vertex))
      {
        throw std::logic_error("invalid command");
      }

      vertexes.pushBack(vertex);
    }

    Graph graph;
    graph.extractFrom(graphs_.get(oldName), vertexes);

    GraphStorage copy(graphs_);
    copy.add(graphName, graph);
    graphs_.swap(copy);
  }
}