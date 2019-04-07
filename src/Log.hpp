#ifndef LOG_H_
#define LOG_H_

#include <chrono>
#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

class Log {
  private:
    std::string m_logFilename = "sdr_log.txt";
    bool m_writeOn;

    Log(const std::string& filename);

    std::mutex m_logMutex;
    std::ofstream m_logFile;

  public:
    ~Log();

    void Msg(const std::string& msg);
    void Error(const std::string& err);
    void LogData(const std::string& logName, double data);

    static void MSG(const std::string& msg) { Log::GetLog()->Msg(msg); }

    static void ERR(const std::string& msg) { Log::GetLog()->Error(msg); }

    static void LOG_DATA(const std::string& logName, double data) {
        Log::GetLog()->LogData(logName, data);
    }

    static Log* GetLog() {
        static Log log("log.txt");
        return &log;
    }
};

#endif
