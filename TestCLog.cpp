#ifdef OS_WINDOWS
#define NOGDI 
#include <Windows.h>
#undef ERROR

#define _CRT_SECURE_NO_WARNINGS
#include <process.h>
unsigned int __stdcall tests(void* pv = NULL);

#else
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void * tests(void* pv = NULL);
#endif

#include "log/CLogs.h"
using namespace OSL;

void multithread_tests( int num );

int main(int argc, char** argv)
{
    // Initialize log
    CLog::InitLog( argv[0], "./log/");

    if( argc > 1 )
        multithread_tests( atoi(argv[1]));
    else
        tests();

    // End log
    CLog::EndLog();

    getchar();
    return 0;
}


void multithread_tests(int num)
{
    int nNum = num > 0 ? num : 10;

    // 创建线程
    for( int i = 0; i < nNum; i++ )
    {
        unsigned int thrId = 0;

#ifdef OS_WINDOWS
        HANDLE hthread = (HANDLE)_beginthreadex( NULL,
            0,
            tests,
            NULL,
            0,
            &thrId );
        if (!hthread)
        {
            LOG_WARN << "Created a thread failed!";
            break;
        }

        DBG_RAWLOG("Created a thread [%d] - %u OK.\n", i, thrId);
        CloseHandle(hthread);
#else
        int nRet = pthread_create( NULL, NULL, tests, (void*)&thrId );
#endif
    }
}
#ifdef OS_WINDOWS
unsigned int __stdcall tests(void* pv)
#else
void * tests(void* pv )
#endif
{
    CLog::SetAlsoLogToStderr(true);

    LOG_INFO  << " Test LOG_INFO  - LOG(INFO)  ";
    LOG_WARN  << " Test LOG_WARN  - LOG(WARN)  ";
    LOG_ERROR << " Test LOG_ERROR - LOG(ERROR) ";
    //LOG_FATAL << " Test LOG_FATAL - LOG(FATAL) ";

    DLOG_INFO  << " Test DLOG_INFO   - DLOG(INFO)   ";
    DLOG_WARN  << " Test DLOG_WARN   - DLOG(WARN)   ";
    DLOG_ERROR << " Test DLOG_ERROR  - DLOG(ERROR)  ";
    //DLOG_FATAL << " Test DLOG_FATAL  - DLOG(FATAL)  ";

    DBG_LOG << " Test DBG_LOG - DLOG(INFO)";

    RAWLOG_INFO (" Test RAWLOG_INFO  : %s, %s, %d", "OSL_LOG::CLog::RawLog(INFO, ",  __FUNCTION__, __LINE__);
    RAWLOG_WARN (" Test RAWLOG_WARN  : %s, %s, %d", "OSL_LOG::CLog::RawLog(WARN, ",  __FUNCTION__, __LINE__);
    RAWLOG_ERROR(" Test RAWLOG_ERROR : %s, %s, %d", "OSL_LOG::CLog::RawLog(ERROR, ", __FUNCTION__, __LINE__);
    //RAWLOG_FATAL("Test RAWLOG_FATAL : %s, %s, %d", "OSL_LOG::CLog::RawLog(FATAL, ", __FILE__, __LINE__);

    DBG_RAWLOG(" Test DBG_RAWLOG : %s, %s, %d", "OSL_LOG::CLog::Debug(", __FUNCTION__, __LINE__);

    // 设置日志级别的日志文件路径和文件名前缀
    //CLog::SetLogDestination(INFO,   "log_info_");     //高于INFO级别的，记录在当前目录并以info_为前缀的日志, //如果前缀为空，则不建立相应级别的日志
    //CLog::SetLogDestination(WARN,   "log_warn_");     //高于WARNING级别的，记录在当前目录并以warn_为前缀的日志
    //CLog::SetLogDestination(ERROR,  "log_err_");      //高于ERROR级别的，记录在当前目录并以err_为前缀的日志

    LOG_INFO  << " Test CLog::SetLogDestination  - log_info_  ";
    LOG_WARN  << " Test CLog::SetLogDestination  - log_info_  ";
    LOG_ERROR  << " Test CLog::SetLogDestination  - log_info_  ";

    //CLog::SetLogPrefix(false);
    LOG_INFO  << " Test CLog::SetLogPrefix(false)  ";

    //CLog::SetLogDestination(INFO, "HAPPY");
    //CLog::SetLogFilenameExtension("extension");
    LOG_INFO  << " Test CLog::SetLogFilenameExtension(\"extension\") ";

    //CLog::SetStderrLogging(INFO);                   //配置输出到标准错误输出的最低日记级别
    LOG_INFO  << " CLog::SetStderrLogging(INFO) ";

    //CLog::SetLogPrefix(true);
    LOG_INFO << "CLog::SetLogPrefix(true)";

    //CLog::SetLogToStderr(true);                                    //只输出到标准错误输出
    LOG_INFO  << " Test LOG_INFO  - CLog::SetLogToStderr true ";
    LOG_WARN  << " Test LOG_WARN  - CLog::SetLogToStderr true ";
    LOG_ERROR << " Test LOG_ERROR - CLog::SetLogToStderr true ";

    //CLog::SetLogToStderr(false);
    LOG_INFO  << " Test LOG_INFO  - CLog::SetLogToStderr false ";
    LOG_WARN  << " Test LOG_WARN  - CLog::SetLogToStderr false ";
    LOG_ERROR << " Test LOG_ERROR - CLog::SetLogToStderr false ";

    //CLog::SetAlsoLogToStderr(true);
    LOG_INFO  << " Test CLog::SetAlsoLogToStderr(true) ";

    CLog::SetAlsoLogToStderr(false);
    //CLog::SetLogToStderr(false);
    //CLog::SetMaxLogSize(10);
    for( int i = 0; i < 20000; ++i)
        RAWLOG_WARN("[%06d]d:/opensource/glog-0.3.1-1.tar/glog-0.3.1/vsprojects/testglog/testclog.cpp,d:/opensource/glog-0.3.1-1.tar/glog-0.3.1/vsprojects/testglog/testclog.cpp,d:/opensource/glog-0.3.1-1.tar/glog-0.3.1/vsprojects/testglog/testclog.cpp,d:/opensource/glog-0.3.1-1.tar/glog-0.3.1/vsprojects/testglog/testclog.cpp,d:/opensource/glog-0.3.1-1.tar/glog-0.3.1/vsprojects/testglog/testclog.cpp,d:/opensource/glog-0.3.1-1.tar/glog-0.3.1/vsprojects/testglog/testclog.cpp,d:/opensource/glog-0.3.1-1.tar/glog-0.3.1/vsprojects/testglog/testclog.cpp,d:/opensource/glog-0.3.1-1.tar/glog-0.3.1/vsprojects/testglog/testclog.cpp,d:/opensource/glog-0.3.1-1.tar/glog-0.3.1/vsprojects/testglog/testclog.cpp\n",
            i+1); 
    LOG_INFO << " Test MaxLogSize and create new log file.";

    return 0;
}
