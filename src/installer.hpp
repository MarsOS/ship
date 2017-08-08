#ifndef SHIP_INSTALLER
#define SHIP_INSTALLER

using namespace std;

#include "info.hpp"

class PackageInstaller
{
public:
  PackageInstaller(string path);
  ~PackageInstaller();
  void install();
protected:
  string path;
  PackageInfo *pi;
};

#endif // SHIP_INSTALLER
