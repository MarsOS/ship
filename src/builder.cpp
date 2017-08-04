#include "builder.hpp"
#include <string.h>

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

  archive_write_close(a);
  archive_write_free(a);
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
  archive_entry_set_size(entry, strlen(content.c_str()));
  archive_entry_set_filetype(entry, AE_IFREG);
  archive_entry_set_perm(entry, perm);

  archive_write_header(this->a, entry);
  archive_write_data(this->a, content.c_str(), strlen(content.c_str()));

  archive_entry_free(entry);
}
