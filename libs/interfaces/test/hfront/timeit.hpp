#ifndef TIME_IT_HPP
#define TIME_IT_HPP

/*
  Copyright(c) 2004, Christopher Diggins
  licensed under the Open Software Licence version 2.0
*/

#include <time.h>

class TimeIt {
  public:
    TimeIt() { mnStart = GetTickCount(); };      
    ~TimeIt() { std::cout << "time elapsed (msec): " << GetMSecElapsed() << std::endl; };
    int GetMSecElapsed() { return GetTickCount() - mnStart; }; 
    int GetTickCount() { return int(double(clock()) * 1000 / CLOCKS_PER_SEC); };
  private:
    int mnStart;
};

#endif // #ifdef TIME_IT_HPP
