#ifndef SHIP_INFO
#define SHIP_INFO

using namespace std;

#include <archive.h>
#include <archive_entry.h>
#include <json.hpp>
using json = nlohmann::json;


class PackageInfo
{
public:
  PackageInfo(string path);
  ~PackageInfo();
  json getBlueprint();
  string getName();
  string getVersion();
  string getArch();
  string getMaintainer();
  string getLicense();
  string getBugtracker();
  string getURL();
  string getPath();
  long long getSize(int scale=1);
  bool isCompatible();
protected:
  string path;
  json blueprint;
  string version;
  string arch;
  long long size;
  struct archive *archive;
  string readEntry();
};

#endif // SHIP_INFO
