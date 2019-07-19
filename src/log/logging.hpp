#pragma once
#include <Debug/BsDebug.h>

/**
 * Wrapper around bs::gDebug().log(...), similar to BS_LOG, which is a bit too verbose.
 *
 * BS_LOG will output the location of the log-call by default,
 * which includes the rather lengthy function signature.
 */
#define REGOTH_LOG(verbosity, category, message, ...)                                   \
  do                                                                                    \
  {                                                                                     \
    using namespace ::bs;                                                               \
    if ((INT32)LogVerbosity::verbosity <= (INT32)BS_LOG_VERBOSITY)                      \
    {                                                                                   \
      gDebug().log(StringUtil::format(message, ##__VA_ARGS__), LogVerbosity::verbosity, \
                   LogCategory##category::_id);                                         \
    }                                                                                   \
  } while (0)
