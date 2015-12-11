#include "logging.h"

#define LOG_MIN_LEVEL WARNING

namespace keyremaplinux {

LogMessage::LogMessage(const char* fname, int line, int severity)
    : fname_(fname), line_(line), severity_(severity) {}

void LogMessage::GenerateLogMessage() {
  if (severity_ >= LOG_MIN_LEVEL) {
    fprintf(stderr, "[%c %s:%d] %s\n", "IWEF"[severity_], fname_, line_,
        str().c_str());
  }
}

LogMessage::~LogMessage() { GenerateLogMessage(); }

LogMessageFatal::LogMessageFatal(const char* file, int line)
    : LogMessage(file, line, FATAL) {}

LogMessageFatal::~LogMessageFatal() {
  GenerateLogMessage();
  abort();
}

}  // end namespace keyremaplinux
