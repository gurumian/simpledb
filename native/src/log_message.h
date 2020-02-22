#ifndef BASE_LOGGING_LOG_MESSAGE_H_
#define BASE_LOGGING_LOG_MESSAGE_H_

#define DO_NOT_USE_GLOG

#if defined(DO_NOT_USE_GLOG)
#if defined(LOG)
#undef LOG
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <sstream>
#include <ostream>
#include <string.h>
#include <pthread.h>
#include <chrono>

#if defined(__APPLE__)
#include <libgen.h>
#endif


typedef int LogSeverity;
const LogSeverity VERBOSE = -1;
const LogSeverity INFO = 0x80;
const LogSeverity WARNING = 0xA0;
const LogSeverity ERROR = 0xC0;
const LogSeverity FATAL = 0xE0;

namespace base {

namespace logging {

class LogMessage {
public:
  LogMessage(LogSeverity severity, const std::string &file, const std::string &func, int line)
  : severity_(severity), file_(file), func_(func), line_(line) {
    static auto start = std::chrono::high_resolution_clock::now();
    stream() << "[" << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() << "]"
        << "[" << std::hex <<  pthread_self() << "]" << std::dec
        << basename((char *)file_.c_str()) << "(" << line_ << ") "
        << func_;
  }

  ~LogMessage() {
    std::cout << stream_.str() << std::endl;
  }

  std::ostream &stream() {return stream_;}

private:
  std::ostringstream stream_;
  LogSeverity severity_;
  const std::string file_;
  const std::string func_;
  const unsigned int line_;
};

} // namespace logging

} // namespace base

#define LOG(verbose_level) ::base::logging::LogMessage(verbose_level, __FILE__, __func__, __LINE__).stream()

#else
#include "glog/logging.h"
#endif

#endif // BASE_LOGGING_LOG_MESSAGE_H_