#ifndef GRAPHS_TABLE_HPP
#define GRAPHS_TABLE_HPP

#include <iosfwd>
#include <string>
#include "graph.hpp"

namespace kondrat
{
  using GraphStorage = HashTable< std::string, Graph, blake2, Equal< std::string > >;

  struct GraphsTable
  {
    void readFile(std::istream & in);

    void graphs(std::istream & in, std::ostream & out, std::string graphName);
    void vertexes(std::istream & in, std::ostream & out, std::string graphName);
    void outbound(std::istream & in, std::ostream & out, std::string graphName);
    void inbound(std::istream & in, std::ostream & out, std::string graphName);
    void bind(std::istream & in, std::ostream & out, std::string graphName);
    void cut(std::istream & in, std::ostream & out, std::string graphName);
    void create(std::istream & in, std::ostream & out, std::string graphName);
    void merge(std::istream & in, std::ostream & out, std::string graphName);
    void extract(std::istream & in, std::ostream & out, std::string graphName);

  private:
    GraphStorage graphs_;
  };
}

#endif