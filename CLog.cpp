/*****************************************************************************************
File name   : CLogs.cpp
Author      : Yin Yong
Version     : V1.0
Date        : 2011-12-1
Description : 基于Google Glog的日志类
Others      : 
History : 
Date      Author        Version          Modification
---------------------------------------------------------------
Date          Author              Version          Modification
2011-12-1     Yin Yong            V1.0             created
2012-03-20    Yin Yong            V1.1             增加类静态变量来控制Debug日志的输出
******************************************************************************************/

#include "CLogs.h"


const size_t kRawLogBufSize = 2048;

namespace OSL
{

#ifdef _DEBUG
    bool CLog::m_bIsDebug = true;
#else
    bool CLog::m_bIsDebug = false;
#endif

    // 初始化日志
    void CLog::InitLog(const char* argv0, const char* dir)
    {
        // 获取不含路径和后缀名的程序名
        const char* slash = strrchr(argv0, '/');
#ifdef _WIN32
        if (!slash)  slash = strrchr(argv0, '\\');
#endif
        slash = slash ? slash + 1 : argv0;
        char buf[1024] = { 0 };
        char* p = strrchr((char*)slash, '.');
        if( p )
            strncpy(buf, slash, (p-slash));
        else
            strcpy(buf, slash);

        // 初始化Google glog
        InitGoogleLogging(buf);

        // 设置日志路径
        SetLogDir(dir);

        // 日志文件只写一个，所以取消Info以外的文件
        SetLogDestination(FATAL,  "");
        SetLogDestination(ERROR,  "");
        SetLogDestination(WARN,   "");

        // 完整的日志文件名称：  <short program name>.<hostname>.log.<date>-<time>.<pid>
        // 拼接日志文件名的前部：<short program name>.<hostname>.log.        
        string hostname;
        GetHostName(&hostname);
        *(buf+strlen(buf)) = '.';
        strcpy(buf+strlen(buf), hostname.c_str());
        strcpy(buf+strlen(buf), ".log.");

        // 设置INFO级别的日志文件前缀
        if( 0 != strcmp(dir, "./"))
        {
            string tmp = dir;
            tmp += buf;
            SetLogDestination(INFO, tmp.c_str());
        }
        else
            SetLogDestination(INFO, buf);
        
        // 默认情况下WARN级别的日志会屏幕输出
        SetStderrThreshold(WARN);

        // 日志是否写屏幕( Debug写，Release不写 )
        //SetAlsoLogToStderr(m_bIsDebug);
        SetLogToStderr(false);
        SetAlsoLogToStderr(false);
    }

    // 结束日志
    void CLog::EndLog(void)
    {
        FlushAllLogFiles();
    }

    // 支持格式化的日志
    void CLog::RawLog( bool checkDebugMode, int severity, const char* fileName, int line, const char* fmt, ... )
    {
        if( checkDebugMode )
        {
            if ( !m_bIsDebug )
                return;
        }

        char buf[kRawLogBufSize] = { 0 };
        size_t size = sizeof(buf);
        va_list ap;
        va_start(ap, fmt);
        int n = vsnprintf(buf, size, fmt, ap);
        va_end(ap);

        if (n < 0 || n > (int)size)
            return ;
        else
            *(buf + n) = 0;

        google::LogMessage(fileName, line, severity).stream() << buf;
    }

    bool CLog::SetMinLogLevel(const string& level)
    {
        if( level.empty() ) return false;
        else return SetMinLogLevel(level.c_str());
    }

    bool CLog::SetMinLogLevel(const char* level)
    {
        if ( !level )
        {
            return false;
        }
        else if (0 == stricmp( level, "debug") )
        {
            SetMinLogLevel(INFO);
            OPEN_DEBUG_LOG();
        }
        else if (0 == stricmp( level, "info") )
        {
            SetMinLogLevel(INFO);
            CLOSE_DEBUG_LOG();
        }
        else if (0 == stricmp( level, "warn") )
        {
            SetMinLogLevel(WARN);
            CLOSE_DEBUG_LOG();
        }
        else if (0 == stricmp( level, "error") )
        {
            SetMinLogLevel(ERROR);
            CLOSE_DEBUG_LOG();
        }
        else if (0 == stricmp( level, "fatal") )
        {
            SetMinLogLevel(FATAL);
            CLOSE_DEBUG_LOG();
        }
        else 
            return false;

        return true;
    }
    
    bool CLog::SetScreenLogLevel(const string& level)
    {
        if( level.empty() ) return false;
        else return SetScreenLogLevel(level.c_str());
    }

    bool CLog::SetScreenLogLevel(const char* level)
    {
        if ( !level )
        {
            return false;
        }
        else if (0 == stricmp( level, "debug") )
        {
            OPEN_DEBUG_LOG();
            SetStderrThreshold(INFO);
        }
        else if (0 == stricmp( level, "info") )
        {
            SetStderrThreshold(INFO);
            CLOSE_DEBUG_LOG();
        }
        else if (0 == stricmp( level, "warn") )
        {
            SetStderrThreshold(WARN);
            CLOSE_DEBUG_LOG();
        }
        else if (0 == stricmp( level, "error") )
        {
            SetStderrThreshold(ERROR);
            CLOSE_DEBUG_LOG();
        }
        else if (0 == stricmp( level, "fatal") )
        {
            SetStderrThreshold(FATAL);
            CLOSE_DEBUG_LOG();
        }
        else return false;

        return true;
    }

    // 获取主机名称 // copied from logging.cc
    void GetHostName(string* hostname)
    {
#if defined(HAVE_SYS_UTSNAME_H)
        struct utsname buf;
        if (0 != uname(&buf)) {
            // ensure null termination on failure
            *buf.nodename = '\0';
        }
        *hostname = buf.nodename;
#elif defined(WIN32)
        char buf[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD len = MAX_COMPUTERNAME_LENGTH + 1;
        if (GetComputerNameA(buf, &len)) {
            *hostname = buf;
        } else {
            hostname->clear();
        }
#else
        //# warning There is no way to retrieve the host name.
        *hostname = "(unknown)";
#endif
    }

} // namespace OSL_LOG
