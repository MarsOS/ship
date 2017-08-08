#include "db.hpp"

using namespace std;

#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <ctime>

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

static string DB_INSERT = R"sql(
  INSERT INTO packages ( name, version, install, change, blueprint, reason, files, size, user, lastuser )
  VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10);

)sql";

DBWrapper::DBWrapper()
{
  int r = sqlite3_open(DBPATH, &this->db);
  if(r)
  {
    cerr << "Error: Could not Open Database: " << endl;
    cerr << sqlite3_errmsg(this->db) << endl;
  }
}

DBWrapper::~DBWrapper()
{
  sqlite3_close(this->db);
}

bool DBWrapper::insertPackage(string name, string version, string bp, int reason, string files, int size, int user)
{
  time_t t_now = std::time(nullptr);
  sqlite3_stmt *ps;
  sqlite3_prepare(this->db, DB_INSERT.c_str(), -1, &ps, NULL);

  sqlite3_bind_text(ps, 1, name.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(ps, 2, version.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(ps, 3, t_now);
  sqlite3_bind_int(ps, 4, t_now);
  sqlite3_bind_text(ps, 5, bp.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(ps, 6, reason);
  sqlite3_bind_text(ps, 7, files.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(ps, 8, size);
  sqlite3_bind_int(ps, 9, user);
  sqlite3_bind_int(ps, 10, user);

  int r = sqlite3_step(ps);
  if (r != SQLITE_DONE)
  {
      cerr << "Error: Inserting Package failed:" <<  sqlite3_errmsg(this->db) << endl;
      return false;
  }
  sqlite3_finalize(ps);
  return true;
}

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

bool DBWrapper::TryLock()
{
  if(DBWrapper::isLocked())
    return false;

  ofstream lockfile(DBLOCKPATH);
  return lockfile.good();
}

bool DBWrapper::TryRelease()
{
  return remove(DBLOCKPATH) == 0;
}
