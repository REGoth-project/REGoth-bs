#pragma once

#include <Error/BsException.h>

namespace REGoth
{
  /**
   * Exception for signaling that an assertion failed.
   */
  class AssertionException : public bs::Exception
  {
  public:
    AssertionException(const bs::String& inDescription, const bs::String& inSource,
                       const char* inFile, long inLine)
        : bs::Exception("AssertionException", inDescription, inSource, inFile, inLine)
    {
    }
  };
}  // namespace REGoth
