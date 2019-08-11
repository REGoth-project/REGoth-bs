#pragma once

#include <BsPrerequisites.h>
#include <String/BsString.h>

/**
 * Simulates an unhandled exception by crashing the application.
 *
 * This is a wrapper around `BS_EXCEPT`, since that one doesn't work without `using namespace bs;`
 * apparently.
 */
#define REGOTH_THROW(exception, message, ...)                         \
  do                                                                  \
  {                                                                   \
    using namespace bs;                                               \
    BS_EXCEPT(exception, StringUtil::format(message, ##__VA_ARGS__)); \
  } while (0)
