#ifndef LOG_HXX
#define LOG_HXX

#include <fstream>

namespace Log {

  class Logger
  {
  private: 
    Logger( const Logger&);
    Logger& operator=( Logger& );
    std::ofstream m_logFile;
  public:
    Logger();
    void write(const std::string &msg);
  };

}

extern Log::Logger g_log;

#endif
