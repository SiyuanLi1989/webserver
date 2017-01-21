#ifndef LOG_H
#define LOG_H
#include <fstream>
#include <iostream>
using namespace std;

class CLog
{
public:
    CLog();
    ~CLog();
    static bool Init();
    static ofstream& LOG(){return m_out;}
private:
    static ofstream m_out;
};
#endif
