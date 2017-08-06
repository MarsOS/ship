#include "info.hpp"
#include <iostream>

using namespace std;

string PackageInfo::readEntry()
{
  ssize_t size;
  string ret = "";
  while(1)
  {
    char buffer[2048] = {0};
    size = archive_read_data(this->archive, buffer, 2048);
    if(size < 0)
    {
      cerr << "Error reading data from archive" << endl;
      return "";
    }
    else if(size == 0)
    {
      return ret;
    }
    ret += buffer;
  }
}

PackageInfo::PackageInfo(string path)
{
  this->path = path;
  this->archive = archive_read_new();
  archive_read_support_filter_gzip(this->archive);
  archive_read_support_format_all(this->archive);
  int r = archive_read_open_filename(this->archive, path.c_str(), 10240);
  if(r != ARCHIVE_OK)
  {
    cerr << "Opening the Package failed: " << path << endl;
    return;
  }

  struct archive_entry *entry;
  while(archive_read_next_header(this->archive, &entry) == ARCHIVE_OK)
  {
    const char *path = archive_entry_pathname(entry);
    if(strncmp(path, "root", 4) == 0)
    {
      archive_read_data_skip(this->archive);
      continue; // We do not want to access the data files
    }
    else if(strncmp(path, "meta", 4) == 0)
    {
      if(strcmp(path, "meta/arch") == 0)
        this->arch = readEntry();
      else if(strcmp(path, "meta/size") == 0)
        this->size = atoll(readEntry().c_str());
      else if(strcmp(path, "meta/version") == 0)
        this->version = readEntry();
      else if(strcmp(path, "meta/blueprint.json") == 0)
        this->blueprint = json::parse(readEntry());
      else
        archive_read_data_skip(this->archive);
    }
    else
    {
      cerr << "Warning: Invalid path detected in package: " << path << endl;
    }
  }
}

PackageInfo::~PackageInfo()
{
  archive_read_free(this->archive);
}

string PackageInfo::getPath() { return this->path; }
string PackageInfo::getVersion() { return this->version; }
string PackageInfo::getArch() { return this->arch; }
long long PackageInfo::getSize(int scale) { return this->size / scale; }
json PackageInfo::getBlueprint() { return this->blueprint; }
//blueprint information
string PackageInfo::getName() { return this->blueprint["name"]; }
string PackageInfo::getLicense() { return this->blueprint["license"]; }
string PackageInfo::getBugtracker() { return this->blueprint["bugtracker"]; }
string PackageInfo::getURL() { return this->blueprint["url"]; }
string PackageInfo::getMaintainer()
{
  string name = this->blueprint["maintainer"]["name"];
  string mail = this->blueprint["maintainer"]["email"];
  return name + " <" + mail + ">";
}
bool PackageInfo::isCompatible() { return (this->arch == "any" || this->arch == ARCH); }
