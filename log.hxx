#ifndef LOG_HXX
#define LOG_HXX

#include <fstream>
#include <mutex>

namespace Log {

  class Logger
  {
  private: 
    Logger( const Logger&);
    Logger& operator=( Logger& );
    std::ofstream m_logFile;
    std::mutex m_mutex;
  public:
    Logger();
    void write(const std::string &msg);
  };

}

extern Log::Logger g_log;

#endif
