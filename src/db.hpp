#ifndef SHIP_DB
#define SHIP_DB

using namespace std;

#include <sqlite3.h>
#include <string>

class DBWrapper
{
public:
  static bool Bootstrap();
  static bool isLocked();
  static bool TryLock();
  static bool TryRelease();

  DBWrapper();
  ~DBWrapper();

  bool insertPackage(string name, string version, string bp, int reason, string files, int size, int user);
  bool updatePackage(string name, string version, string bp, string files, int size, int user);
  bool packageExists(string name);
protected:
  sqlite3 *db;
};

#endif // SHIP_DB
