#include "log.h"
#include <time.h>
ofstream CLog::m_out;
CLog::CLog()
{
}
CLog::~CLog()
{
}
bool
CLog::Init()
{
    char str_time[100];
    time_t localTime = time(NULL);
    struct tm *curTime = NULL;
    curTime = localtime(&localTime);
    cout<<localTime<<endl;
    strftime(str_time, sizeof(str_time), "WebServer-LOG-%Y%m%d_%H%M%S", curTime);
    cout<<str_time<<endl;
//    m_out.open(str_time);
	return true;
}
