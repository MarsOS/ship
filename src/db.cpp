#include "db.hpp"

using namespace std;

#include <iostream>
#include <fstream>
#include <sqlite3.h>

static string DB_BOOTSTRAP = R"sql(
  CREATE TABLE packages (
    id      INTEGER PRIMARY KEY NOT NULL,
    name    TEXT NOT NULL,
    version TEXT NOT NULL,
    install INTEGER NOT NULL,
    change  INTEGER NOT NULL,
    blueprint TEXT NOT NULL,
    reason    INTEGER NOT NULL,
    files     TEXT NOT NULL,
    size      INTEGER NOT NULL,
    user      INTEGER NOT NULL,
    lastuser  INTEGER NOT NULL
)
)sql";

bool DBWrapper::Bootstrap()
{
  sqlite3 *db;
  int r = sqlite3_open(DBPATH, &db);
  if(r)
  {
    cerr << "Error: Could not Bootstrap Database: " << endl;
    cerr << sqlite3_errmsg(db) << endl;
    return false;
  }

  char *err;
  sqlite3_exec(db, DB_BOOTSTRAP.c_str(), NULL, NULL, &err);

  sqlite3_close(db);
  return true;
}

bool DBWrapper::isLocked()
{
  ifstream db(DBLOCKPATH);
  return db.good();
}
