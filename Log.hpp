#ifndef __LOG__HPP__
#define __LOG__HPP__

#include <iostream>
#include <string>
#include <sys/time.h>
using namespace std;
#define INFO 0
#define DEBUG 1
#define WARNING 2
#define ERROR 3

time_t GetTimeStamp()
{
	struct timeval time;
	gettimeofday(&time,NULL);
	return time.tv_sec;
}

#define LOG(level_,message_) Log(level_,message_,__FILE__,__LINE__)
void Log(int level_,string message_,string file_,int  line_)
{
		cout<<" [ "<<level_<<" : "<<message_<<"] "<<"[ "<<GetTimeStamp()<<"] "<<"[ "<<file_<<" : "<<line_<<"]"<<endl;
}

#endif // __LOG_HPP__

