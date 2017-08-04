#include "builder.hpp"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <list>

static char TEMPDIR_TEMPLATE[] =  "/tmp/marsos.ship.build.XXXXXXXX";

list<string> *fileList;
unsigned long long total_size = 0;

PackageBuilder::PackageBuilder (string name, json blueprint)
{
  this->blueprint = blueprint;
  this->a = archive_write_new();
  archive_write_add_filter_gzip(this->a);
  archive_write_set_format_pax_restricted(this->a);

  string fn = name + ".tar.gz";
  archive_write_open_filename(this->a, fn.c_str());
}

void PackageBuilder::build()
{
  this->addMetaFile("blueprint.json", this->blueprint.dump());

  char *pkg_temp = mkdtemp(TEMPDIR_TEMPLATE);
  if(pkg_temp == NULL)
  {
    cerr << "Could not allocate a temp directory, Exiting." << endl;
    exit(1);
  }

  setenv("pkgdir", pkg_temp, 0);
  setenv("srcdir", "./", 0);
  setenv("arch", ARCH, 0);

  cout << "=> Build environment is ready" << endl;

  this->runStage("config");
  this->runStage("build");
  this->runStage("package");

  cout << "=> Build stages are complete" << endl;

  this->addDir(pkg_temp);

  cout << "=> All files are added to the archive" << endl;

  this->addMetaFile("size", to_string(total_size));

  cout << "=> Meta information collected" << endl;

  archive_write_close(a);
  archive_write_free(a);

  cout << "=> Packge was successuflly build!" << endl;
}

void PackageBuilder::runStage(string stage)
{
  if(this->blueprint[stage].is_array() == false)
  {
    cerr << "Warning: Stage '" << stage << "' is not defined, skipping." << endl;
    return;
  }
  cout << "  -> Executing Stage \'" << stage << "\'" << endl;
  for(size_t x = 0; x < blueprint[stage].size(); x++)
  {
    string task = blueprint[stage][x];
    cout << "    -> Executing: " << task << endl;
    system(task.c_str());
  }
}

void PackageBuilder::addMetaFile(string name, string content)
{
  this->addFile("meta/" + name, content);
}

void PackageBuilder::addFile(string path, string content, bool s, string realpath, int perm)
{
  struct archive_entry *entry;
  entry = archive_entry_new();
  archive_entry_set_pathname(entry, path.c_str());
  archive_entry_set_filetype(entry, AE_IFREG);

  if(!s) // Do not call stat on realpath
  {
    archive_entry_set_perm(entry, perm);
  }
  else
  {
    struct stat fs;
    if(stat(realpath.c_str(), &fs) < 0)
    {
      cerr << "Stat failed for " << realpath << endl;
      exit(1);
    }
    archive_entry_set_perm(entry, fs.st_mode);

    // Get file content
    ifstream in;
    in.open(realpath, ios::in | ios::binary);
    stringstream strs;
    strs << in.rdbuf();
    content = strs.str();
  }

  size_t len = content.size();

  archive_entry_set_size(entry, len);

  archive_write_header(this->a, entry);
  archive_write_data(this->a, content.c_str(), len);

  archive_entry_free(entry);
}

static int buildList(const char *name, const struct stat *status, int type)
{
  if(type != FTW_F)
    return 0;

  fileList->push_back(name);
  total_size += status->st_size;
  return 0;
}

void PackageBuilder::addDir(string realpath, string dest)
{
  fileList = new list<string>();
  ftw(realpath.c_str(), buildList , 1);
  while(fileList->size() > 0)
  {
    string realName = fileList->front();
    string baseName = realName;
    baseName.replace(0, realpath.length(), "root");

    this->addFile(baseName, "", true, realName);
    fileList->pop_front();
  }
}
