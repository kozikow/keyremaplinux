#ifndef KEYREMAPLINUX_UTIL_LOGGING
#define KEYREMAPLINUX_UTIL_LOGGING

#include <sstream>

namespace keyremaplinux {

const int INFO = 0;
const int WARNING = 1;
const int ERROR = 2;
const int FATAL = 3;
const int NUM_SEVERITIES = 4;

class LogMessage : public std::basic_ostringstream<char> {
 public:
  LogMessage(const char* fname, int line, int severity);
  ~LogMessage();

 protected:
  void GenerateLogMessage();

 private:
  const char* fname_;
  int line_;
  int severity_;
};

// LogMessageFatal ensures the process will exit in failure after
// logging this message.
class LogMessageFatal : public LogMessage {
 public:
  LogMessageFatal(const char* file, int line);
  ~LogMessageFatal();
};

#define _LOG_INFO \
  ::keyremaplinux::LogMessage(__FILE__, __LINE__, keyremaplinux::INFO)
#define _LOG_WARNING \
  ::keyremaplinux::LogMessage(__FILE__, __LINE__, keyremaplinux::WARNING)
#define _LOG_ERROR \
  ::keyremaplinux::LogMessage(__FILE__, __LINE__, keyremaplinux::ERROR)
#define _LOG_FATAL \
  ::keyremaplinux::LogMessageFatal(__FILE__, __LINE__)

#define LOG(severity) _LOG_##severity

#define CHECK(condition) \
  if (!(condition)) \
  LOG(FATAL) << "Check failed: " #condition " "

}  // end namespace keyremaplinux

#endif // KEYREMAPLINUX_UTIL_LOGGING
