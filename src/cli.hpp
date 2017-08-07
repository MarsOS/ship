#ifndef SHIP_CLI
#define SHIP_CLI

using namespace std;

#include <string>

class CLI
{
public:
  static string KeyValueFormater(string key, string value, int spacing=12);
  static string CompatabilityCheck(bool check, string good="[Compatible]", string bad="[Invalid]");
  static string Header(string header, int spacing=6);
};

#endif // SHIP_CLI
