#include "DaedalusStack.hpp"

namespace REGoth
{
  namespace Scripting
  {
    void DaedalusStack::pushInt(bs::INT32 value)
    {
      mStackInt.emplace_back();
      mStackInt.back().isVariableValue = false;
      mStackInt.back().value           = value;
    }

    void DaedalusStack::pushIntVariable(SymbolIndex symbol, bs::UINT32 arrayIndex)
    {
      mStackInt.emplace_back();
      mStackInt.back().isVariableValue     = true;
      mStackInt.back().variable.symbol     = symbol;
      mStackInt.back().variable.arrayIndex = arrayIndex;
    }

    void DaedalusStack::pushFloat(float value)
    {
      mStackFloat.emplace_back();
      mStackFloat.back().isVariableValue = false;
      mStackFloat.back().value           = value;
    }

    void DaedalusStack::pushFloatVariable(SymbolIndex symbol, bs::UINT32 arrayIndex)
    {
      mStackFloat.emplace_back();
      mStackFloat.back().isVariableValue     = true;
      mStackFloat.back().variable.symbol     = symbol;
      mStackFloat.back().variable.arrayIndex = arrayIndex;
    }

    void DaedalusStack::pushString(bs::String value)
    {
      mStackString.emplace_back();
      mStackString.back().isVariableValue = false;
      mStackString.back().value           = value;
    }

    void DaedalusStack::pushStringVariable(SymbolIndex symbol, bs::UINT32 arrayIndex)
    {
      mStackString.emplace_back();
      mStackString.back().isVariableValue     = true;
      mStackString.back().variable.symbol     = symbol;
      mStackString.back().variable.arrayIndex = arrayIndex;
    }

    void DaedalusStack::pushInstance(SymbolIndex symbol)
    {
      mStackInstance.push_back(symbol);
    }

    void DaedalusStack::pushFunction(SymbolIndex symbol)
    {
      mStackInstance.push_back(symbol);
    }

    bool DaedalusStack::isTopOfIntStackVariable() const
    {
      if (mStackInt.empty())
      {
        // Gothic defaults to returning 0 on an empty stack, which is not a variable
        return false;
      }

      return mStackInt.back().isVariableValue;
    }

    bool DaedalusStack::isTopOfFloatStackVariable() const
    {
      if (mStackFloat.empty())
      {
        // Gothic defaults to returning 0 on an empty stack, which is not a variable
        return false;
      }

      return mStackFloat.back().isVariableValue;
    }

    bool DaedalusStack::isTopOfStringStackVariable() const
    {
      if (mStackString.empty())
      {
        // Gothic defaults to returning 0 on an empty stack, which is not a variable
        return false;
      }

      return mStackString.back().isVariableValue;
    }

    bs::INT32 DaedalusStack::popInt()
    {
      if (mStackInt.empty())
      {
        // Gothic defaults to returning 0 on an empty stack
        return 0;
      }

      if (isTopOfIntStackVariable())
      {
        REGOTH_THROW(
            InvalidParametersException,
            "Top of script stack is a variable, but we were expecting it to be a simple integer!");
      }

      bs::INT32 v = mStackInt.back().value;

      mStackInt.pop_back();

      return v;
    }

    float DaedalusStack::popFloat()
    {
      if (mStackFloat.empty())
      {
        // Gothic defaults to returning 0 on an empty stack
        return 0;
      }

      if (isTopOfStringStackVariable())
      {
        REGOTH_THROW(
            InvalidParametersException,
            "Top of script stack is a variable, but we were expecting it to be a simple float!");
      }

      float v = mStackFloat.back().value;

      mStackFloat.pop_back();

      return v;
    }

    bs::String DaedalusStack::popString()
    {
      if (mStackString.empty())
      {
        // Gothic defaults to returning 0 on an empty stack, so we just guess ""?
        return "";
      }

      if (isTopOfStringStackVariable())
      {
        REGOTH_THROW(
            InvalidParametersException,
            "Top of script stack is a variable, but we were expecting it to be a simple string!");
      }

      bs::String v = mStackString.back().value;

      mStackString.pop_back();

      return v;
    }

    SymbolIndex DaedalusStack::popInstance()
    {
      if (mStackInstance.empty())
      {
        return SYMBOL_INDEX_INVALID;
      }

      SymbolIndex v = mStackInstance.back();

      mStackInstance.pop_back();

      return v;
    }

    SymbolIndex DaedalusStack::popFunction()
    {
      if (mStackFunction.empty())
      {
        return SYMBOL_INDEX_INVALID;
      }

      SymbolIndex v = mStackFunction.back();

      mStackFunction.pop_back();

      return v;
    }

    DaedalusStack::StackVariableValue DaedalusStack::popIntVariable()
    {
      if (!isTopOfIntStackVariable())
      {
        REGOTH_THROW(
            InvalidParametersException,
            "Top of script stack is a simple integer, but we were expecting it to be a variable!");
      }

      return mStackInt.back().variable;
    }

    DaedalusStack::StackVariableValue DaedalusStack::popFloatVariable()
    {
      if (!isTopOfFloatStackVariable())
      {
        REGOTH_THROW(
            InvalidParametersException,
            "Top of script stack is a simple float, but we were expecting it to be a variable!");
      }

      return mStackFloat.back().variable;
    }

    DaedalusStack::StackVariableValue DaedalusStack::popStringVariable()
    {
      if (!isTopOfStringStackVariable())
      {
        REGOTH_THROW(
            InvalidParametersException,
            "Top of script stack is a simple string, but we were expecting it to be a variable!");
      }

      return mStackString.back().variable;
    }
  }  // namespace Scripting
}  // namespace REGoth
