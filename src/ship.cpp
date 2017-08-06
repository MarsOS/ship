#include <iostream>

#include <fstream>
#include <string>

#include <stdio.h>

#include "builder.hpp"
#include "info.hpp"

#include <json.hpp>
using json = nlohmann::json;

using namespace std;

void printHelp()
{
  cout << "// ship  -  Package Manager for MarsOS" << endl << endl;
  cout << "   help      \tprint this help output" << endl;
  cout << "version      \tprint version information" << endl;
  cout << endl;
  cout << "  build      \tbuild a package from source" << endl;
  cout << "   info  <P> \tshow information about a package" << endl;
  cout << "bluepint <P> \tshow the blueprint which was used to build the package" << endl;
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

void getInfo(int argc, char **argv)
{
  if(argc <= 2)
  {
    printHelp();
    exit(1);
  }
  PackageInfo p = PackageInfo(argv[2]);
  cout << "      Package Information       " << endl;
  cout << "--------------------------------" << endl;
  cout << "Package     : " << p.getName() << "\t Version: " << p.getVersion() << endl;
  cout << "Size        : " << p.getSize(1000) << " KB" << endl;
  cout << "License     : " << p.getLicense() << endl;
  cout << "Architecture: " << p.getArch() << (p.isCompatible() ? " [Compatible]" : " [Invalid]") << endl;
  cout << "Project URL : " << p.getURL() << endl;
  cout << endl;
  cout << "If you encounter problems with this package, please contact its maintainer:" << endl;
  cout << "  " << p.getMaintainer() << endl;
  cout << "If you encounter other Bugs, please report it to the Bucktracker:" << endl;
  cout << "  " << p.getBugtracker() << endl;
}

void showBlueprint(int argc, char **argv)
{
  if(argc <= 2)
  {
    printHelp();
    exit(1);
  }
  PackageInfo p = PackageInfo(argv[2]);
  cout << p.getBlueprint().dump(4) << endl;
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
  else if(strcmp(argv[1], "info") == 0)
    getInfo(argc, argv);
  else if(strcmp(argv[1], "blueprint") == 0)
    showBlueprint(argc, argv);

  return 0;
}
