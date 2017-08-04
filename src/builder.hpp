#ifndef SHIP_BUILDER
#define SHIP_BUILDER

using namespace std;

#include <archive.h>
#include <archive_entry.h>
#include <json.hpp>
using json = nlohmann::json;


class PackageBuilder
{
public:
  PackageBuilder(string name, json blueprint);
  void build();
protected:
  void addMetaFile(string name, string content);
  void addFile(string path, string content, bool s=false, string realpath="", int perm=0644);
  struct archive *a;
  json blueprint;
};

#endif // SHIP_BUILDER
