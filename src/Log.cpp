#include <cerrno>
#include <cstring>
#include <ctime>
#include <iostream>
#include <mutex>
#include <thread>

#include "Log.hpp"

Log::Log(const std::string& filename) : m_writeOn(false) {
    if (m_logFile) {
        m_logFile.open(filename, std::ofstream::out);
        if (!m_logFile.is_open()) {
            throw std::runtime_error(
                std::string("Failed to open log file, error: ") +
                std::strerror(errno));
        }
    }
}

Log::~Log() {
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void Log::Error(const std::string& err) {
    std::string finalMsg = std::string("ERROR: ") + err + "\n";
    std::lock_guard<std::mutex> lock(m_logMutex);
    if (m_writeOn)
        m_logFile << finalMsg;
    std::cerr << finalMsg;
}

void Log::Msg(const std::string& msg) {
    std::string finalMsg = std::string("MSG : ") + msg + "\n";
    std::lock_guard<std::mutex> lock(m_logMutex);
    if (m_writeOn)
        m_logFile << finalMsg;
    std::cout << finalMsg;
}

void Log::LogData(const std::string& logName, double data) {
    m_logFile.write((char*)&data, sizeof(double));
}
