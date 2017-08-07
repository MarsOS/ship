#ifndef SHIP_DB
#define SHIP_DB

using namespace std;



class DBWrapper
{
public:
  static bool Bootstrap();
  static bool isLocked();
};

#endif // SHIP_DB
