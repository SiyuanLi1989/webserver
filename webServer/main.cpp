#include "log.h"
#include <iostream>
#include "net.h"

int main()
{
    CLog::Init();
    NetServer myserver;
    myserver.Start();
    return 0;
}
