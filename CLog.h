/*****************************************************************************************
File name   : CLogs.h
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
/*
 This class file is based on Google Glog library

 ** LOG SEVERITY:
    INFO, WARNING, ERROR, and FATAL.

 // Very important: logging a message at the FATAL severity level causes
 // the program to terminate (after the message is logged).)

 ** LOG FILENAME:
 <program name>.<hostname>.log.<date>.<time>.<pid>

 ** LOG LINE PREFIX FORMAT:
     Lmmdd hh:mm:ss.uuuuuu threadid file:line] msg...

 where the fields are defined as follows:
   L                A single character, representing the log level
                    (eg 'I' for INFO)
   mm               The month (zero padded; ie May is '05')
   dd               The day (zero padded)
   hh:mm:ss.uuuuuu  Time in hours, minutes and fractional seconds
   threadid         The space-padded thread ID as returned by GetTID()
                    (this matches the PID on Linux)
   file             The file name
   line             The line number
   msg              The user-supplied message
*/

#ifndef __OSL_CLOGS_H__
#define __OSL_CLOGS_H__

#include <string>
using namespace std;

#ifdef NEED_FILE_LINE_
#define _LOG__FILE__  __FILE__
#define _LOG__LINE__	__LINE__
#else
#define _LOG__FILE__	""
#define _LOG__LINE__	0
#endif

#ifdef OS_WINDOWS
#define NOGDI 
#include <Windows.h>
#undef ERROR
#pragma warning(disable:4244)
#pragma warning(disable:4996)

#else // Linux

#define stricmp strcasecmp
#endif

#include "glog/logging.h"

// 打开/关闭 Debug日志，打开时写文件，关闭时不写。
#define OPEN_DEBUG_LOG()   OSL::CLog::SetDebugFlag(true)
#define CLOSE_DEBUG_LOG()  OSL::CLog::SetDebugFlag(false)
#define IS_OPEN_DEBUG()    OSL::CLog::IsDebug()

namespace OSL
{
    // redefine some macro called GLOG macro
    #define LOG_INFO    LOG(INFO)
    #define LOG_WARN    LOG(WARNING)
    #define LOG_ERROR   LOG(ERROR)
    #define LOG_FATAL   LOG(FATAL)

#define LOG_SCRN_FILE_INFO(str) do {		\
    OSL::CLog::SetAlsoLogToStderr(true);	\
    LOG_INFO << str;						\
    OSL::CLog::SetAlsoLogToStderr(false);	\
    } while(0)


#define LOG_F_INFO(str) LOG_INFO << str

    #define DLOG_INFO   DLOG(INFO)
    #define DLOG_WARN   DLOG(WARNING)
    #define DLOG_ERROR  DLOG(ERROR)
    #define DLOG_FATAL  DLOG(FATAL)

    //#define DBG_LOG     DLOG(INFO)
    #define DBG_LOG     if(IS_OPEN_DEBUG()) LOG(INFO)

    #define RAWLOG_INFO(...)   OSL::CLog::RawLog(false, google::INFO,    _LOG__FILE__, _LOG__LINE__, __VA_ARGS__)
    #define RAWLOG_WARN(...)   OSL::CLog::RawLog(false, google::WARNING, _LOG__FILE__, _LOG__LINE__, __VA_ARGS__)
    #define RAWLOG_ERROR(...)  OSL::CLog::RawLog(false, google::ERROR,   _LOG__FILE__, _LOG__LINE__, __VA_ARGS__)
    #define RAWLOG_FATAL(...)  OSL::CLog::RawLog(false, google::FATAL,   _LOG__FILE__, _LOG__LINE__, __VA_ARGS__)

    #define DBG_RAWLOG(...)    OSL::CLog::RawLog(true, google::INFO, _LOG__FILE__, _LOG__LINE__, __VA_ARGS__)

    const int INFO  = google::INFO;
    const int WARN  = google::WARNING;
    const int ERROR = google::ERROR;
    const int FATAL = google::FATAL;


    // 日志类
    class CLog
    {
    public:
        // Initialize/End the Log
        static void InitLog(const char* argv0, const char* dir = "./");
        static void EndLog(void);

        // support C format log
        static void RawLog(
            bool checkDebugMode,
            int  severity,
            const char* fileName,
            int line, 
            const char* fmt, ... );

        /**********************************************************************
        set GLOG global variables or call GLOG global function
        **********************************************************************/

        // Initialize google's logging library. You will see the program name
        // specified by argv0 in log outputs.
        static void InitGoogleLogging(const char* argv0) { google::InitGoogleLogging(argv0); }

        //
        // Set the destination to which a particular severity level of log
        // messages is sent.  If base_filename is "", it means "don't log this
        // severity".  Thread-safe.
        //
        static void SetLogDestination(int severity, const char* base_filename) { google::SetLogDestination( severity, base_filename ); }

        //
        // Specify an "extension" added to the filename specified via
        // SetLogDestination.  This applies to all severity levels.  It's
        // often used to append the port we're listening on to the logfile
        // name.  Thread-safe.
        //
        static void SetLogFilenameExtension(const char* ext) { google::SetLogFilenameExtension( ext ); }

        //
        // Make it so that all log messages of at least a particular severity
        // are logged to stderr (in addition to logging to the usual log
        // file(s)).  Thread-safe.
        //
        static void SetStderrLogging(int min_severity) { google::SetStderrLogging( min_severity ); }

        //
        // Make it so that all log messages go only to stderr.  Thread-safe.
        //
        static void LogToStderr();

        // Flushes all log files that contains messages that are at least of
        // the specified severity level.  Thread-safe.
        static void FlushLogFiles(int min_severity) { google::FlushLogFiles( min_severity ); }
        static void FlushAllLogFiles() { google::FlushLogFiles( INFO ); }

        // Flushes all log files that contains messages that are at least of
        // the specified severity level. Thread-hostile because it ignores
        // locking -- used for catastrophic failures.
        static void FlushLogFilesUnsafe(int min_severity) { google::FlushLogFilesUnsafe( min_severity ); }

        /**********************************************************************
        SET some GLOG flags
        **********************************************************************/

        // FLAGS_log_dir
        // If specified, logfiles are written into this directory instead of the
        // default logging directory.
        static void SetLogDir(const string& dir = string("./")) { FLAGS_log_dir = dir; }

        // FLAGS_max_log_size
        // Sets the maximum log file size (in MB). default 1800MB
        static void SetMaxLogSize(int size) { FLAGS_max_log_size = size; }

        // FLAGS_logtostderr
        // Set whether log messages go to stderr instead of logfiles
        static void SetLogToStderr(bool b = true) {  FLAGS_logtostderr = b; }

        // FLAGS_alsologtostderr
        // Set whether log messages go to stderr in addition to logfiles.
        static void SetAlsoLogToStderr(bool b = true) {  FLAGS_alsologtostderr = b;}

        // FLAGS_stderrthreshold
        // Log messages at a level >= this flag are automatically sent to
        // stderr in addition to log files.
        static void SetStderrThreshold(int level) { FLAGS_stderrthreshold = level; }

        // FLAGS_log_prefix
        // Set whether the log prefix should be prepended to each line of output.
        static void SetLogPrefix(bool b = true) { FLAGS_log_prefix = b; }

        // FLAGS_logbuflevel
        // Log messages at a level <= this flag are buffered.
        // Log messages at a higher level are flushed immediately.
        static void SetLogBufLevel(int level) { FLAGS_logbuflevel = level; }

        // FLAGS_logbufsecs
        // Sets the maximum number of seconds which logs may be buffered for.
        static void SetLogBufSecs(int secs) { FLAGS_logbufsecs = secs; }

        // FLAGS_minloglevel
        // Log suppression level: messages logged at a lower level than this
        // are suppressed.
        static void SetMinLogLevel(int level) { FLAGS_minloglevel = level; }

        // Sets whether to avoid logging to the disk if the disk is full.
        // DECLARE_bool(stop_logging_if_full_disk);

        // Sets the path of the directory into which to put additional links
        // to the log files.
        //DECLARE_string(log_link);
        //DECLARE_int32(v);  // in vlog_is_on.cc

        // 查看/设置Debug标志函数
        static bool IsDebug(void) { return m_bIsDebug; }
        static void SetDebugFlag(bool flag)
        {
            m_bIsDebug = flag;
        }
        
        // 设置屏幕日志的输出级别
        static bool SetScreenLogLevel(const char* level);
        static bool SetScreenLogLevel(const string& level);

        // 设置文件 log 级别
        static bool SetMinLogLevel(const char* level);
        static bool SetMinLogLevel(const string& level);

    private:
        CLog() {};
        virtual ~CLog(){};
        static bool m_bIsDebug;

    protected:
        //std::string m_strLogFileName;       // 日志文件名
        //std::string m_strLogDir;            // 日志所在的目录/文件夹
        //std::string m_strLogPath;           // 日志文件全路径
        //size_t m_nMaxSize;                  // 最大日志文件大小，单位MB
    };

    // 获取主机名称
    void GetHostName(string* hostname);

} // namespace OSL

#endif // __OSL_CLOGS_H__
