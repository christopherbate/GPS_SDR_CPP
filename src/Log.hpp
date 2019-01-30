#ifndef LOG_H_
#define LOG_H_

#include <chrono>
#include <fstream>
#include <mutex>
#include <string>

class Log {
  private:
    std::string m_logFilename = "mznt_log.txt";
    bool m_writeOn;

    Log(const std::string& filename);

    std::mutex m_logMutex;
    std::ofstream m_logFile;

  public:
    ~Log();

    void Msg(const std::string& msg);
    void Error(const std::string& err);

    static void MSG(const std::string& msg) { Log::GetLog()->Msg(msg); }

    static void ERR(const std::string& msg) { Log::GetLog()->Error(msg); }

    static Log* GetLog() {
        static std::time_t result = std::time(nullptr);
        static Log log("log.txt" +
                       std::string(std::asctime(std::localtime(&result))));
        return &log;
    }
};

#endif
