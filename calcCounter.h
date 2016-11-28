
#if (defined __unix__) || (defined __APPLE__)

#include "stdlib.h"
#include "sys/time.h"

class calcCounter
{
protected:
    
    long startCountSec,stopCountSec,startCountMicroSec,stopCountMicroSec;
    
public:
    void StartCounter();
    void StopCounter();
    double GetElapsedTime();
    
    calcCounter()
    {
        StartCounter();
    }
    
};

inline void calcCounter::StartCounter()
{
    struct timeval tv;
    
    gettimeofday(&tv,NULL);
    
    startCountSec = tv.tv_sec;
    startCountMicroSec = tv.tv_usec;
    
}

inline void calcCounter::StopCounter()
{
    struct timeval tv;
    
    gettimeofday(&tv,NULL);
    
    stopCountSec = tv.tv_sec;
    stopCountMicroSec = tv.tv_usec;
}


inline double calcCounter::GetElapsedTime()
{
    float elapsedTime = 1.0 * (stopCountSec-startCountSec) + 1E-6 * (stopCountMicroSec - startCountMicroSec);
    return elapsedTime;
}

#endif