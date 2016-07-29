/********************************************************
        File name       : log.h
        Author          : CHEN Tao
        Created Time    : 29/07/2016
********************************************************/


#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;


#ifdef LOG_ON
#define LogInfo(msg) Log::GetInstance()->writeLog(INFO,msg)
#define LogWarn(msg) Log::GetInstance()->writeLog(WARN,msg)
#define LogError(msg) Log::GetInstance()->writeLog(ERROR,msg)
#else
#define LogInfo(msg)
#define LogWarn(msg)
#define LogError(msg)
#endif


enum Log_type {
    INFO,
    WARN,
    ERROR
};

class Log;

static Log* logInstance;

class Log
{
public:

	static Log* GetInstance()
	{
		if(logInstance == NULL){
			logInstance = new Log();
		}

		return logInstance;
	}


	Log()
	{
		ofs.open ("LOG.txt", std::ofstream::out | std::ofstream::app);
	}

	void writeLog(Log_type type, string msg){
		
		cout<<"[" + getLabel(type) + "]"<<msg<<endl;
		ofs <<"[" + getLabel(type) + "]"<<msg<<endl;	
	}

	~Log()
	{
		ofs.close();
	};

private:
	std::ofstream ofs;
	inline string getLabel(Log_type type) {
        string label;
        switch(type) {
            case INFO:  label = "INFO"; break;
            case WARN:  label = "WARN"; break;
            case ERROR: label = "ERROR"; break;
        }
        return label;
    }

};

#endif // LOG_H
