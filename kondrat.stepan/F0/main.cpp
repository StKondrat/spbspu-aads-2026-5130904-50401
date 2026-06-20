#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "cuckoo-hash-table.hpp"
#include "hash-func.hpp"
#include "matrix-table.hpp"

int main()
{
  using Command = void(kondrat::MatrixTable::*)(std::istream &, std::ostream &, std::string);
  using CommandTable = kondrat::CuckooHashTable< std::string, Command, kondrat::Hash1, kondrat::Hash2 >;

  kondrat::MatrixTable table;
  CommandTable commands;

  commands.add("create", &kondrat::MatrixTable::create);
  commands.add("drop", &kondrat::MatrixTable::drop);
  commands.add("show", &kondrat::MatrixTable::show);
  commands.add("list", &kondrat::MatrixTable::list);
  commands.add("rename", &kondrat::MatrixTable::rename);
  commands.add("set", &kondrat::MatrixTable::set);
  commands.add("add-many", &kondrat::MatrixTable::addMany);
  commands.add("sub-many", &kondrat::MatrixTable::subMany);
  commands.add("mul-many", &kondrat::MatrixTable::mulMany);
  commands.add("mul-number", &kondrat::MatrixTable::mulNumber);
  commands.add("pow", &kondrat::MatrixTable::pow);
  commands.add("insert-row", &kondrat::MatrixTable::insertRow);
  commands.add("insert-col", &kondrat::MatrixTable::insertCol);
  commands.add("insert", &kondrat::MatrixTable::insert);
  commands.add("concat-row", &kondrat::MatrixTable::concatRow);
  commands.add("concat-col", &kondrat::MatrixTable::concatCol);
  commands.add("concat-main-diag", &kondrat::MatrixTable::concatMainDiag);
  commands.add("concat-side-diag", &kondrat::MatrixTable::concatSideDiag);
  commands.add("minor", &kondrat::MatrixTable::minor);
  commands.add("det", &kondrat::MatrixTable::det);
  commands.add("rank", &kondrat::MatrixTable::rank);
  commands.add("compare", &kondrat::MatrixTable::compare);
  commands.add("save-matrix", &kondrat::MatrixTable::saveMatrix);
  commands.add("load-matrix", &kondrat::MatrixTable::loadMatrix);

  std::string command;

  while (std::cin >> command)
  {
    if (command == "exit")
    {
      break;
    }

    try
    {
      const Command method = commands.get(command);
      std::string matrixName;
      if (command != "list" && !(std::cin >> matrixName))
      {
        throw std::logic_error("invalid command");
      }
      (table.*method)(std::cin, std::cout, matrixName);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
    }

    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
