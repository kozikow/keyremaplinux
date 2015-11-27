#include "logging.h"

namespace keyremaplinux {

LogMessage::LogMessage(const char* fname, int line, int severity)
    : fname_(fname), line_(line), severity_(severity) {}

void LogMessage::GenerateLogMessage() {
  fprintf(stderr, "[%c %s:%d] %s\n", "IWEF"[severity_], fname_, line_,
          str().c_str());
}

LogMessage::~LogMessage() { GenerateLogMessage(); }

LogMessageFatal::LogMessageFatal(const char* file, int line)
    : LogMessage(file, line, FATAL) {}

LogMessageFatal::~LogMessageFatal() {
  GenerateLogMessage();
  abort();
}

}  // end namespace keyremaplinux
