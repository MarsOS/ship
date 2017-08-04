#include <iostream>

#include <fstream>
#include <string>

#include <stdio.h>

#include "builder.hpp"

#include <json.hpp>
using json = nlohmann::json;

using namespace std;

void printHelp()
{
  cout << "// ship  -  Package Manager for MarsOS" << endl << endl;
  cout << "   help \tprint this help output" << endl;
  cout << "version \tprint version information" << endl;
  exit(0);
}

void printVersion()
{
  cout << "// ship  -  Package Manager for MarsOS" << endl << endl;
  cout << "     Version: " << VERSION << endl;
  cout << "      Commit: " << COMMIT << endl;
  cout << "Architecture: " << ARCH << endl;
  exit(0);
}

void buildPackage(int argc, char **argv)
{
  ifstream blueprint("blueprint.json");

  if(!blueprint.is_open())
  {
    cerr << "Could not open Blueprint file, abort" << endl;
    exit(1);
  }

  json bp;
  blueprint >> bp;

  PackageBuilder pb = PackageBuilder(bp["name"],  bp);
  pb.build();
}

int main(int argc, char *argv[])
{
  if(argc <= 1)
    printHelp();

  if(strcmp(argv[1], "help") == 0)
    printHelp();
  else if(strcmp(argv[1], "version") == 0)
    printVersion();
  else if(strcmp(argv[1], "build") == 0)
    buildPackage(argc, argv);

  return 0;
}
