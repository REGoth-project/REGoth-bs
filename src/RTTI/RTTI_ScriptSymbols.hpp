#pragma once

#include "RTTI_TypeIDs.hpp"
#include <BsCorePrerequisites.h>
#include <Private/RTTI/BsGameObjectRTTI.h>  // Says private, but bs:f uses this too in their RTTIs
#include <Reflection/BsRTTIType.h>
#include <scripting/ScriptSymbols.hpp>

#define REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(classname)     \
                                                      \
public:                                               \
  RTTI_##classname()                                  \
  {                                                   \
  }                                                   \
                                                      \
  bs::SPtr<bs::IReflectable> newRTTIObject() override \
  {                                                   \
    return bs::bs_shared_ptr_new<classname>();        \
  }                                                   \
                                                      \
  const bs::String& getRTTIName() override            \
  {                                                   \
    static bs::String name = #classname;              \
    return name;                                      \
  }                                                   \
                                                      \
  bs::UINT32 getRTTIId() override                     \
  {                                                   \
    return TID_REGOTH_Script##classname;              \
  }

namespace REGoth
{
  namespace Scripting
  {
    using UINT32 = bs::UINT32;

    class RTTI_SymbolBase : public bs::RTTIType<SymbolBase, bs::IReflectable, RTTI_SymbolBase>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(name, 0)
      BS_RTTI_MEMBER_PLAIN(type, 1)
      BS_RTTI_MEMBER_PLAIN(index, 2)
      BS_RTTI_MEMBER_PLAIN(parent, 3)
      BS_RTTI_MEMBER_PLAIN(isClassVar, 4)
      BS_RTTI_MEMBER_PLAIN(isKeptAfterLoad, 5)
      BS_END_RTTI_MEMBERS

      REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(SymbolBase)
    };

    class RTTI_SymbolInt : public bs::RTTIType<SymbolInt, SymbolBase, RTTI_SymbolInt>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN_ARRAY_NAMED(ints, ints.values, 0)
      BS_END_RTTI_MEMBERS

      REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(SymbolInt)
    };

    class RTTI_SymbolFloat : public bs::RTTIType<SymbolFloat, SymbolBase, RTTI_SymbolFloat>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN_ARRAY_NAMED(floats, floats.values, 0)
      BS_END_RTTI_MEMBERS

      REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(SymbolFloat)
    };

    class RTTI_SymbolString : public bs::RTTIType<SymbolString, SymbolBase, RTTI_SymbolString>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN_ARRAY_NAMED(strings, strings.values, 0)
      BS_END_RTTI_MEMBERS

      REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(SymbolString)
    };

    class RTTI_SymbolClass : public bs::RTTIType<SymbolClass, SymbolBase, RTTI_SymbolClass>
    {
      BS_BEGIN_RTTI_MEMBERS
      // None
      BS_END_RTTI_MEMBERS

      REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(SymbolClass)
    };

    class RTTI_SymbolScriptFunction : public bs::RTTIType<SymbolScriptFunction, SymbolBase, RTTI_SymbolScriptFunction>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(address, 0)
      BS_END_RTTI_MEMBERS

      REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(SymbolScriptFunction)
    };

    class RTTI_SymbolExternalFunction : public bs::RTTIType<SymbolExternalFunction, SymbolBase, RTTI_SymbolExternalFunction>
    {
      BS_BEGIN_RTTI_MEMBERS
      // None
      BS_END_RTTI_MEMBERS

      REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(SymbolExternalFunction)
    };

    class RTTI_SymbolPrototype : public bs::RTTIType<SymbolPrototype, SymbolBase, RTTI_SymbolPrototype>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(constructorAddress, 0)
      BS_END_RTTI_MEMBERS

      REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(SymbolPrototype)
    };

    class RTTI_SymbolInstance : public bs::RTTIType<SymbolInstance, SymbolBase, RTTI_SymbolInstance>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_PLAIN(constructorAddress, 0)
      BS_RTTI_MEMBER_PLAIN(instance, 1)
      BS_END_RTTI_MEMBERS

      REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(SymbolInstance)
    };

    class RTTI_SymbolUnsupported : public bs::RTTIType<SymbolUnsupported, SymbolBase, RTTI_SymbolUnsupported>
    {
      BS_BEGIN_RTTI_MEMBERS
      // None
      BS_END_RTTI_MEMBERS

      REGOTH_RTTI_SCRIPT_SYMBOL_GLUE(SymbolUnsupported)
    };
  }  // namespace Scripting
  // namespace Scripting
}  // namespace REGoth
