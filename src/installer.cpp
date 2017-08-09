#include "installer.hpp"

using namespace std;

#include "db.hpp"
#include "cli.hpp"

#include <unistd.h>
#include <sys/types.h>

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


  cout << "=> Updating Package Database" << endl;

  DBWrapper db = DBWrapper();
  if(db.packageExists(this->pi->getName()))
  {
    cout << "=> Package allready installed, Updating" << endl;
    db.updatePackage(
      this->pi->getName(),
      this->pi->getVersion(),
      this->pi->getBlueprint().dump(),
      "",
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
      "",
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
