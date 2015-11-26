#include "logging.h"

namespace keyremaplinux {

LogMessage::LogMessage(const char* fname, int line, int severity)
    : fname_(fname), line_(line), severity_(severity) {}

void LogMessage::GenerateLogMessage() {
  // TODO(jeff,sanjay): For open source version, replace this with something
  // that logs through the env or something and fill in appropriate time info.
  fprintf(stderr, "%c %s:%d] %s\n", "IWEF"[severity_], fname_, line_,
          str().c_str());
}

LogMessage::~LogMessage() { GenerateLogMessage(); }

LogMessageFatal::LogMessageFatal(const char* file, int line)
    : LogMessage(file, line, FATAL) {}

LogMessageFatal::~LogMessageFatal() {
  // abort() ensures we don't return (we promised we would not via
  // ATTRIBUTE_NORETURN).
  GenerateLogMessage();
  abort();
}

}  // end namespace keyremaplinux
