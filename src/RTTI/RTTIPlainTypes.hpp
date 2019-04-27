#pragma once
#include <BsPrerequisites.h>

#include <scripting/ScriptTypes.hpp>
#include "RTTI_TypeIDs.hpp"

namespace bs
{
  /**
   * RTTIPlainType for std::pair.
   *
   * @see		RTTIPlainType
   */
  template <class T>
  struct RTTIPlainType<REGoth::Scripting::ScriptValues<T>>
  {
    enum
    {
      id = REGoth::TID_REGOTH_ScriptInts
    };

    enum
    {
      hasDynamicSize = 1
    };

    /** @copydoc RTTIPlainType::toMemory */
    static void toMemory(const REGoth::Scripting::ScriptValues<T>& data, char* memory)
    {
      UINT32 size       = sizeof(UINT32);
      char* memoryStart = memory;
      memory += sizeof(UINT32);

      UINT32 firstSize = rttiGetElemSize(data.values);
      RTTIPlainType<bs::Vector<T>>::toMemory(data.values, memory);

      memory += firstSize;
      size += firstSize;

      memcpy(memoryStart, &size, sizeof(UINT32));
    }

    /** @copydoc RTTIPlainType::fromMemory */
    static UINT32 fromMemory(REGoth::Scripting::ScriptValues<T>& data, char* memory)
    {
      UINT32 size = 0;
      memcpy(&size, memory, sizeof(UINT32));
      memory += sizeof(UINT32);

      UINT32 firstSize = RTTIPlainType<bs::Vector<T>>::fromMemory(data.values, memory);
      memory += firstSize;

      return size;
    }

    /** @copydoc RTTIPlainType::getDynamicSize */
    static UINT32 getDynamicSize(const REGoth::Scripting::ScriptValues<T>& data)
    {
      UINT64 dataSize = sizeof(UINT32);
      dataSize += rttiGetElemSize(data.values);

      assert(dataSize <= std::numeric_limits<UINT32>::max());

      return (UINT32)dataSize;
    }
  };

}
// namespace REGoth
