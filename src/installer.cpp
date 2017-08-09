#include "installer.hpp"

using namespace std;

#include "db.hpp"
#include "cli.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <archive.h>


PackageInstaller::PackageInstaller(string path)
{
  this->path = path;
  this->pi = new PackageInfo(path);

}

void PackageInstaller::install()
{
  cout << CLI::KeyValueFormater("Package Name", this->pi->getName()) << endl;
  cout << CLI::KeyValueFormater("Package Version", this->pi->getVersion()) << endl;
  cout << CLI::KeyValueFormater("Package Size", to_string(this->pi->getSize(1000))) << " KB" << endl;

  if(!this->pi->isCompatible())
  {
    cerr << "Package is not compatible with your architecture." << endl;
    return;
  }

  cout << CLI::KeyValueFormater("Compatible: ", "True") << endl;

  if(!DBWrapper::TryLock())
  {
    cerr << "Could not lock Database!" << endl;
    return;
  }

  cout << "=> Locked Database" << endl;


  cout << "=> Extracting Files" << endl;

  struct archive *pkg;
  struct archive *out;
  struct archive_entry *entry;

  pkg = archive_read_new();
  archive_read_support_format_all(pkg);
  archive_read_support_filter_gzip(pkg);

  int r = archive_read_open_filename(pkg, this->pi->getPath().c_str(), 10240);
  if(r)
  {
    cerr << "Error: Could not open package file to extract" << endl;
    exit(1);
  }

  out = archive_write_disk_new();
  archive_write_disk_set_options(out, ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS);
  archive_write_disk_set_standard_lookup(out);

  string files = "";

  while(1)
  {
    r = archive_read_next_header(pkg, &entry);
    if(r == ARCHIVE_EOF)
      break;
    else if(r < ARCHIVE_OK)
      cerr << "Warning: " << archive_error_string(pkg);

    string path = archive_entry_pathname(entry);
    if(path.compare(0, 4, "root") != 0)
    {
      archive_read_data_skip(pkg);
      continue;
    }

    path.replace(0, 4, INSTALLPATH);
    files += path + ";";
    archive_entry_set_pathname(entry, path.c_str());

    r = archive_write_header(out, entry);
    if(r < ARCHIVE_OK)
    {
      cerr << "Warning: " << archive_error_string(out) << endl;
    }
    else if (archive_entry_size(entry) > 0)
    {
      const void *buff;
      size_t size;
      long offset;
      while(1)
      {
        r = archive_read_data_block(pkg, &buff, &size, &offset);
        if(r == ARCHIVE_EOF)
        {
          break;
        }
        if(r < ARCHIVE_OK)
        {
          cerr << "Error: " << archive_error_string(pkg);
          break;
        }

        r = archive_write_data_block(out, buff, size, offset);
        if(r < ARCHIVE_OK)
        {
          cerr << "Error: " << archive_error_string(out);
          break;
        }
      }
    }
  }

  cout << "=> Updating Package Database" << endl;

  DBWrapper db = DBWrapper();
  if(db.packageExists(this->pi->getName()))
  {
    cout << "=> Package allready installed, Updating" << endl;
    db.updatePackage(
      this->pi->getName(),
      this->pi->getVersion(),
      this->pi->getBlueprint().dump(),
      files,
      this->pi->getSize(),
      getuid()
    );
  }
  else
  {
    db.insertPackage(
      this->pi->getName(),
      this->pi->getVersion(),
      this->pi->getBlueprint().dump(),
      0,
      files,
      this->pi->getSize(),
      getuid()
    );
  }

  cout << "=> Releasing Database Lock" << endl;

  DBWrapper::TryRelease();
}

PackageInstaller::~PackageInstaller()
{
  delete this->pi;
}
