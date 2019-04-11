/**\file
 */
#pragma once
#include <BsPrerequisites.h>
#include <scripting/ScriptVM.hpp>

namespace REGoth
{
  namespace Scripting
  {
    /**
     * This is the stack of the Daedalus-VM.
     *
     * In the original, the stack was a single list of 32-bit integers
     * which was used for everything. This is not unusual for a processor to do
     * as memory is just bytes anyways. However, to save us from all the casting
     * and string related hacks, this Daedalus-Stack is using multiple stacks under
     * the hood for the different types of values.
     *
     * This keeps the VM type-save internally.
     */
    class DaedalusStack
    {
    public:
      /**
       * Push a simple value onto the stack.
       */
      void pushInt(bs::INT32 value);
      void pushFloat(float value);
      void pushString(bs::String value);
      void pushInstance(SymbolIndex symbol);
      void pushFunction(SymbolIndex symbol);

      /**
       * Push a reference to a variable onto the stack.
       *
       * Attention! This does not check whether the type of the symbol is correct!
       */
      void pushIntVariable(SymbolIndex symbol, bs::UINT32 arrayIndex);
      void pushFloatVariable(SymbolIndex symbol, bs::UINT32 arrayIndex);
      void pushStringVariable(SymbolIndex symbol, bs::UINT32 arrayIndex);

      /**
       * @return Whether the top of the stack is a variable (otherwise it's a pure data value)
       */
      bool isTopOfIntStackVariable() const;
      bool isTopOfFloatStackVariable() const;
      bool isTopOfStringStackVariable() const;

      /**
       * Pops an int-value from the stack.
       *
       * Throws if this value on top is NOT a pure data value. To check that, use
       * isTopOfIntStackVariable(). If it IS a variable, use popIntVariable() instead.
       */
      bs::INT32 popInt();

      /**
       * Pops a float-value from the stack.
       *
       * Throws if this value on top is NOT a pure data value. To check that, use
       * isTopOfFloatStackVariable(). If it IS a variable, use popFloatVariable() instead.
       */
      float popFloat();

      /**
       * Pops an string-value from the stack.
       *
       * Throws if this value on top is NOT a pure data value. To check that, use
       * isTopOfStringStackVariable(). If it IS a variable, use popStringVariable() instead.
       */
      bs::String popString();

      /**
       * Pops from the stack.
       */
      SymbolIndex popInstance();
      SymbolIndex popFunction();

      /**
       * Reference to a value from a variable for use on the stack.
       */
      struct StackVariableValue
      {
        SymbolIndex symbol;
        bs::UINT32 arrayIndex;
      };

      /**
       * Pops an int-variable from the stack.
       *
       * Throws if this value on top is NOT a variable. To check that, use
       * isTopOfIntStackVariable(). If it IS a pure data value, use popInt() instead.
       */
      StackVariableValue popIntVariable();

      /**
       * Pops an float-variable from the stack.
       *
       * Throws if this value on top is NOT a variable. To check that, use
       * isTopOfFloatStackVariable(). If it IS a pure data value, use popFloat() instead.
       */
      StackVariableValue popFloatVariable();

      /**
       * Pops an string-variable from the stack.
       *
       * Throws if this value on top is NOT a variable. To check that, use
       * isTopOfStringStackVariable(). If it IS a pure data value, use popInt() instead.
       */
      StackVariableValue popStringVariable();

    private:
      /**
       * Integer value to push onto the stack. Can be either a plain integer value
       * or a variable value, which we have to look up in the symbol storage first.
       */
      struct StackInt
      {
        bool isVariableValue;
        union {
          StackVariableValue variable;
          bs::INT32 value;
        };
      };

      /**
       * Integer value to push onto the stack. Can be either a plain integer value
       * or a variable value, which we have to look up in the symbol storage first.
       */
      struct StackFloat
      {
        bool isVariableValue;
        union {
          StackVariableValue variable;
          float value;
        };
      };

      /**
       * Integer value to push onto the stack. Can be either a plain integer value
       * or a variable value, which we have to look up in the symbol storage first.
       */
      struct StackString
      {
        bool isVariableValue;

        // Not a union to keep the default constructor
        StackVariableValue variable;
        bs::String value;
      };

      bs::Vector<StackInt> mStackInt;
      bs::Vector<StackFloat> mStackFloat;
      bs::Vector<StackString> mStackString;
      bs::Vector<SymbolIndex> mStackInstance;
      bs::Vector<SymbolIndex> mStackFunction;
    };
  }  // namespace Scripting
}  // namespace REGoth
