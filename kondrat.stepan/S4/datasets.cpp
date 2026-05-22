#include "datasets.hpp"

#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>

namespace kondrat
{
  void print(std::istream & in, std::ostream & out, Collection & datasets)
  {
    std::string name;

    if (!(in >> name))
    {
      throw std::logic_error("invalid command");
    }

    Dictionary & dict = datasets.get(name);

    if (dict.empty())
    {
      out << "<EMPTY>\n";
      return;
    }

    out << name;

    for (Dictionary::iterator it = dict.begin(); it != dict.end(); ++it)
    {
      out << ' ' << it->first << ' ' << it->second;
    }

    out << '\n';
  }

  Dictionary makeComplement(const Dictionary & lhs, const Dictionary & rhs)
  {
    Dictionary result;

    for (Dictionary::const_iterator it = lhs.begin(); it != lhs.end(); ++it)
    {
      if (!rhs.has(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  Dictionary makeIntersect(const Dictionary & lhs, const Dictionary & rhs)
  {
    Dictionary result;

    for (Dictionary::const_iterator it = lhs.begin(); it != lhs.end(); ++it)
    {
      if (rhs.has(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  Dictionary makeUnion(const Dictionary & lhs, const Dictionary & rhs)
  {
    Dictionary result;

    for (Dictionary::const_iterator it = lhs.begin(); it != lhs.end(); ++it)
    {
      result.push(it->first, it->second);
    }

    for (Dictionary::const_iterator it = rhs.begin(); it != rhs.end(); ++it)
    {
      if (!result.has(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  void complement(std::istream & in, std::ostream &, Collection & datasets)
  {
    std::string newName;
    std::string lhsName;
    std::string rhsName;

    if (!(in >> newName >> lhsName >> rhsName))
    {
      throw std::logic_error("invalid command");
    }

    Dictionary result = makeComplement(datasets.get(lhsName), datasets.get(rhsName));
    datasets.push(newName, result);
  }

  void intersect(std::istream & in, std::ostream &, Collection & datasets)
  {
    std::string newName;
    std::string lhsName;
    std::string rhsName;

    if (!(in >> newName >> lhsName >> rhsName))
    {
      throw std::logic_error("invalid command");
    }

    Dictionary result = makeIntersect(datasets.get(lhsName), datasets.get(rhsName));
    datasets.push(newName, result);
  }

  void unionCollections(std::istream & in, std::ostream &, Collection & datasets)
  {
    std::string newName;
    std::string lhsName;
    std::string rhsName;

    if (!(in >> newName >> lhsName >> rhsName))
    {
      throw std::logic_error("invalid command");
    }

    Dictionary result = makeUnion(datasets.get(lhsName), datasets.get(rhsName));
    datasets.push(newName, result);
  }
}