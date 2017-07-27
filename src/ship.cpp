#include <iostream>
#include <string.h>
#include <fstream>
#include <string>
#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>

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

void addEntry(struct archive *a, const char *filename, const char *content, int perm=0644)
{
  struct archive_entry *entry;
  entry = archive_entry_new();
  archive_entry_set_pathname(entry, filename);
  archive_entry_set_size(entry, strlen(content));
  archive_entry_set_filetype(entry, AE_IFREG);
  archive_entry_set_perm(entry, perm);

  archive_write_header(a, entry);
  archive_write_data(a, content, strlen(content));

  archive_entry_free(entry);
}

void buildPackage(int argc, char **argv)
{
  struct archive *a;


  a = archive_write_new();
  archive_write_add_filter_gzip(a);
  archive_write_set_format_pax_restricted(a);
  archive_write_open_filename(a, "test.tar.gz");

  ifstream blueprint("blueprint.json");

  if(!blueprint.is_open())
  {
    cerr << "Could not open Blueprint file, abort" << endl;
    exit(1);
  }

  json bp;
  blueprint >> bp;
  blueprint.clear();
  blueprint.seekg(0);

  std::string blueprint_content((std::istreambuf_iterator<char>(blueprint)), std::istreambuf_iterator<char>());
  addEntry(a, "meta/blueprint.json", blueprint_content.c_str());


  archive_write_close(a);
  archive_write_free(a);

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
