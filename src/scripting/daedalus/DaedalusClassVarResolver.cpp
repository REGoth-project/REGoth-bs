#include "DaedalusClassVarResolver.hpp"

namespace REGoth
{
  namespace Scripting
  {
    DaedalusClassVarResolver::DaedalusClassVarResolver(const ScriptSymbolStorage& scriptSymbols,
                                                       ScriptObjectStorage& scriptObjects)
        : mScriptSymbols(scriptSymbols)
        , mScriptObjects(scriptObjects)
    {
    }

    ScriptInts& DaedalusClassVarResolver::resolveClassVariableInts(
        const bs::String& memberSymbolName)
    {
      throwIfCurrentInstanceNotMatching(memberSymbolName);

      ScriptObject& obj = getCurrentInstanceObject();
      bs::String member = demangleMemberName(memberSymbolName);

      auto it = obj.ints.find(member);

      if (it == obj.ints.end())
      {
        REGOTH_THROW(InvalidParametersException, "Current Instance Object of class " +
                                                     obj.className + " does not have member " +
                                                     member + " of type INT.");
      }

      return it->second;
    }

    bs::UINT32& DaedalusClassVarResolver::resolveClassVariableFunctionPointer(
        const bs::String& memberSymbolName)
    {
      throwIfCurrentInstanceNotMatching(memberSymbolName);

      ScriptObject& obj = getCurrentInstanceObject();
      bs::String member = demangleMemberName(memberSymbolName);

      auto it = obj.functionPointers.find(member);

      if (it == obj.functionPointers.end())
      {
        REGOTH_THROW(InvalidParametersException, "Current Instance Object of class " +
                                                     obj.className + " does not have member " +
                                                     member + " of type FUNCTION POINTER.");
      }

      return it->second;
    }

    ScriptFloats& DaedalusClassVarResolver::resolveClassVariableFloats(
        const bs::String& memberSymbolName)
    {
      throwIfCurrentInstanceNotMatching(memberSymbolName);

      ScriptObject& obj = getCurrentInstanceObject();
      bs::String member = demangleMemberName(memberSymbolName);

      auto it = obj.floats.find(member);

      if (it == obj.floats.end())
      {
        REGOTH_THROW(InvalidParametersException, "Current Instance Object of class " +
                                                     obj.className + " does not have member " +
                                                     member + " of type FLOAT.");
      }

      return it->second;
    }

    ScriptStrings& DaedalusClassVarResolver::resolveClassVariableStrings(
        const bs::String& memberSymbolName)
    {
      throwIfCurrentInstanceNotMatching(memberSymbolName);

      ScriptObject& obj = getCurrentInstanceObject();
      bs::String member = demangleMemberName(memberSymbolName);

      auto it = obj.strings.find(member);

      if (it == obj.strings.end())
      {
        REGOTH_THROW(InvalidParametersException, "Current Instance Object of class " +
                                                     obj.className + " does not have member " +
                                                     member + " of type STRING.");
      }

      return it->second;
    }

    bool DaedalusClassVarResolver::isCurrentInstanceValid() const
    {
      if (mCurrentInstance == SCRIPT_OBJECT_HANDLE_INVALID) return false;

      if (mScriptObjects.isDestroyed(mCurrentInstance)) return false;

      return true;
    }

    bool DaedalusClassVarResolver::isCurrentInstanceOfClass(const bs::String& className) const
    {
      if (!isCurrentInstanceValid()) return false;

      return true;
    }

    ScriptObject& DaedalusClassVarResolver::getCurrentInstanceObject()
    {
      throwIfCurrentInstanceIsInvalid();

      return mScriptObjects.get(mCurrentInstance);
    }

    void DaedalusClassVarResolver::setCurrentInstance(ScriptObjectHandle handle)
    {
      mCurrentInstance = handle;
    }

    void DaedalusClassVarResolver::throwIfCurrentInstanceNotOfClass(
        const bs::String& className) const
    {
      if (!isCurrentInstanceValid())
      {
        REGOTH_THROW(InvalidParametersException,
                     "Current Instance is not valid! Expected it to be a " + className);
      }

      if (!isCurrentInstanceOfClass(className))
      {
        bs::String actual = mScriptObjects.get(mCurrentInstance).className;
        REGOTH_THROW(InvalidParametersException,
                     "Current Instance is a " + actual + ". But we expected " + className + "!");
      }
    }

    void DaedalusClassVarResolver::throwIfCurrentInstanceIsInvalid() const
    {
      if (!isCurrentInstanceValid())
      {
        REGOTH_THROW(InvalidStateException, "Current Instance is not valid!");
      }
    }

    void DaedalusClassVarResolver::throwIfCurrentInstanceNotMatching(
        const bs::String& memberSymbolName) const
    {
      bs::String expected = extractClassNameFromMemberSymbol(memberSymbolName);

      throwIfCurrentInstanceNotOfClass(expected);
    }

    bs::String DaedalusClassVarResolver::demangleMemberName(const bs::String& memberSymbolName)
    {
      auto nameParts = bs::StringUtil::split(memberSymbolName, ".");

      if (nameParts.size() != 2) return memberSymbolName;

      return nameParts[1];
    }

    bs::String DaedalusClassVarResolver::extractClassNameFromMemberSymbol(
        const bs::String& memberSymbolName) const
    {
      auto nameParts = bs::StringUtil::split(memberSymbolName, ".");

      if (nameParts.size() != 2)
      {
        REGOTH_THROW(InvalidParametersException,
                     memberSymbolName + " does not look like a Member Symbol Name.");
      }

      return nameParts[0];
    }
  }  // namespace Scripting
}  // namespace REGoth
