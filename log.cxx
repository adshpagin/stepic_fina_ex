#include <iostream>
#include <ctime>

#include "log.hxx"

#define LOG_FILE_NAME "/tmp/exam_app.log"

Log::Logger g_log;

Log::Logger::Logger()
  : m_logFile(LOG_FILE_NAME, std::ios_base::app)
{}

Log::Logger::Logger( const Log::Logger&)
{}

Log::Logger& Log::Logger::operator=( Log::Logger& logger)
{
  // Formally to reduce warning: no return statement in function returning non-void [-Wreturn-type]
  return logger;
}

void Log::Logger::write(const std::string &msg)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  char buffer[256];

  std::strftime (buffer, 256,"%c", &tm);

  m_logFile << buffer << " " << msg << std::endl;

}
