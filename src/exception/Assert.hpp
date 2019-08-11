#pragma once

#include <String/BsString.h>

#include <exception/AssertionException.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>

#define REGOTH_INTERNAL_ASSERT_MSG(expr, msg, ...)                                                \
  bs::StringUtil::format(bs::String("Assertion failed:  " + bs::String(msg) + ".  Condition: '" + \
                                    bs::String(#expr) + "'"),                                     \
                         ##__VA_ARGS__)

#define REGOTH_INTERNAL_ASSERT_VAL_MSG(var, expr, msg, ...)                                      \
  bs::StringUtil::format(                                                                        \
      bs::String("Assertion for '" + bs::String(#var) + "' (= " + bs::toString(var) +            \
                 ") failed:  " + bs::String(msg) + ".  Condition: '" + bs::String(#expr) + "'"), \
      ##__VA_ARGS__)

// Basic assertion definitions for debug builds.
#ifdef REGOTH_ENABLE_ASSERTIONS
#define REGOTH_ASSERT(expr, msg, ...)                                            \
  do                                                                             \
  {                                                                              \
    if (!(expr))                                                                 \
    {                                                                            \
      bs::String message = REGOTH_INTERNAL_ASSERT_MSG(expr, msg, ##__VA_ARGS__); \
      REGOTH_THROW(REGoth::AssertionException, message);                         \
    }                                                                            \
  } while (false)

#define REGOTH_ASSERT_VAL(var, expr, msg, ...)                                            \
  do                                                                                      \
  {                                                                                       \
    if (!(expr))                                                                          \
    {                                                                                     \
      bs::String message = REGOTH_INTERNAL_ASSERT_VAL_MSG(var, expr, msg, ##__VA_ARGS__); \
      REGOTH_THROW(REGoth::AssertionException, message);                                  \
    }                                                                                     \
  } while (false)
// Disables assertions for non-debug builds.
#else
#define REGOTH_ASSERT(expr, msg, ...)                                            \
  do                                                                             \
  {                                                                              \
    if (!(expr))                                                                 \
    {                                                                            \
      bs::String message = REGOTH_INTERNAL_ASSERT_MSG(expr, msg, ##__VA_ARGS__); \
      REGOTH_LOG(Error, Uncategorized, message);                                 \
    }                                                                            \
  } while (false)

#define REGOTH_ASSERT_VAL(var, expr, msg, ...)                                          \
  do                                                                                    \
  {                                                                                     \
    bs::String message = REGOTH_INTERNAL_ASSERT_VAL_MSG(var, expr, msg, ##__VA_ARGS__); \
    REGOTH_LOG(Error, Uncategorized, message);                                          \
  } while (false)
#endif
