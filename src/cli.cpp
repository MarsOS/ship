#include "cli.hpp"

using namespace std;

#define BOLD "\e[1m"
#define RESET_BOLD "\e[21m"

#define UNDERLINE "\e[4m"
#define RESET_UNDERLINE "\e[24m"

#define COLOR_RED "\e[91m"
#define COLOR_GREEN "\e[92m"
#define RESET_COLOR "\e[39m"

string CLI::KeyValueFormater(string key, string value, int spacing)
{
  string ret = "";
  for(int x = spacing - key.length(); x > 0; x--)
    ret += " ";

  ret += BOLD + key + RESET_BOLD;

  ret += ":  " + value;
  return ret;
}

string CLI::CompatabilityCheck(bool check, string good, string bad)
{
  string ret = "";
  if (check)
    ret += COLOR_GREEN + good + RESET_COLOR;
  else
    ret += COLOR_RED + bad + RESET_COLOR;

  return ret;
}

string CLI::Header(string header, int spacing)
{
  string ret = "";
  for(;spacing > 0; spacing--)
    ret += " ";

  ret += UNDERLINE + header + RESET_UNDERLINE;

  return ret;
}
