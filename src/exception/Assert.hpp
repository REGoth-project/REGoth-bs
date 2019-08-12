#pragma once

#include <String/BsString.h>

#include <exception/AssertionException.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>

/**
 * This file defines assertion macros for development purposes.
 *
 * Note: Certain CMake build types may disable assertions completely due to performance reasons.  It
 * should *never* be relied on assertions for productive builds.
 *
 * If assertions are enabled (through the definition of `REGOTH_ENABLE_ASSERTIONS`, assertions that
 * fail will raise a `REGoth::AssertionException`.  If the assertions are disabled (default), failed
 * assertions will result in a log of the severity "Error".
 *
 * Examples:
 *
 * - Assert that the variable `value` is in the range [0, 10):
 *
 * ```cpp
 * float value = getValueToCheck();
 * REGOTH_ASSERT_VAL(value, 0 <= value && value < 10, "Must be in [0, 10)");
 * ```
 *
 * If this assertion fails, the name of the inspected variable will be printed, together with its
 * actual value and a stack trace.
 *
 * - Assert that a function evaluates to true:
 *
 * ```cpp
 * REGOTH_ASSERT(hopefullyTrueFunction(), "Must evaluate to true");
 * ```
 *
 * - Assert that a expression is true with formatting:
 *
 * ```cpp
 * SomeStruct s1 = getSomeStruct();
 * SomeStruct s2 = getAnotherStruct();
 * REGOTH_ASSERT(s1 == s2, "'{0}' and '{1}' must match", s1.getDescriptiveName(),
 *               s2.getDescriptiveName());
 * ```
 */

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
#else /* ifndef REGOTH_ENABLE_ASSERTIONS */
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
