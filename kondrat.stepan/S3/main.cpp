#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include "graphs-table.hpp"
#include "hash-table.hpp"
#include "hash-func.hpp"

int main(int argc, char ** argv)
{
  using namespace kondrat;

  if (argc != 2)
  {
    std::cerr << "invalid arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "cannot open file\n";
    return 1;
  }

  GraphsTable table;

  try
  {
    table.readFile(file);
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  using cmd_t = void (GraphsTable::*)(std::istream &, std::ostream &, std::string);

  HashTable< std::string, cmd_t, blake2, Equal< std::string > > commands;

  commands.add("graphs", &GraphsTable::graphs);
  commands.add("vertexes", &GraphsTable::vertexes);
  commands.add("outbound", &GraphsTable::outbound);
  commands.add("inbound", &GraphsTable::inbound);
  commands.add("bind", &GraphsTable::bind);
  commands.add("cut", &GraphsTable::cut);
  commands.add("create", &GraphsTable::create);
  commands.add("merge", &GraphsTable::merge);
  commands.add("extract", &GraphsTable::extract);

  std::string cmd;

  while (std::cin >> cmd)
  {
    std::string graphName;

    if (cmd != "graphs")
    {
      if (!(std::cin >> graphName))
      {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        continue;
      }
    }

    std::string args;
    std::getline(std::cin, args);
    std::istringstream input(args);

    try
    {
      cmd_t command = commands.get(cmd);
      (table.*command)(input, std::cout, graphName);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
