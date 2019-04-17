Case-Study: Daedalus VM
=======================
    
Implementation of a virtual machine able to execute Daedalus Bytecode.

Daedalus is the scripting language used in the original gothic games.
It was made for the use within the ZenGin specifically and is not used outside
of the Gothic games. Hence, the language and bytecode format are proprietary.


DaedalusVM in ZenLib
--------------------

This is a re-write of the DaedalusVM found inside ZenLib which might get replaced
by this one at some point. The reason for rewriting is that we want to keep the
door open for a possible different scripting backend to be used by REGoth. The
ZenLib's DaedalusVM is not structured in a way which makes that possible.


Design decisions
----------------

Unlinke the DaedalusVM in ZenLib, this VM will handle script symbols very differently.
While previously we had created native classes for each script class we needed to
access from engine code, this VM uses general ``key``/``value`` pairs to store
data from script instances. This saves us from maintaining a copy of every script class
which might even be different between games or mods.

It will also make the internal differences between global variables, arrays and classes
simpler.


Parts of a DaedalusVM
---------------------

To execute daedalus code, the VM needs the following basic parts:

- Instruction Memory
- Instruction Interpreter
- Instruction Executer
- Symbol Data Storage
- External function mapping
- Global registers
- Stack

Each of these will be discussed in the further sections.


Instruction Memory
~~~~~~~~~~~~~~~~~~~

The Instruction Memory is where the compiled byte code is stored and waiting
to be executed. The byte code itself is compiled from text files by the original
game and stored inside a .DAT-file, which can be read by ZenLib.

In daedalus byte code a single instruction can be made up from multiple bytes. However,
as instructions are interpreted on the fly, there is no need to further process
the byte code to split it into the real instruction counterparts. Another reason for not doing
that is, that jump target will always reference raw byte code addresses.


Instruction Interpreter
~~~~~~~~~~~~~~~~~~~~~~~

This part is what interprets the bytecode to make the actual instruction from multiple bytes.
It was decoupled from the actual execution part for a better software strucutre and better
debugging possibilities.

The interpreter can be given a raw byte code address and it will try to pack all information
from the instruction so the execution stage can work with it.


Instruction Executor
~~~~~~~~~~~~~~~~~~~~

Once an instruction has been interpreted, this part will execute it. Executing an instruction
will modify the internal state of the VM. For example, an ``ADD``-instruction will pop two
values from the stack, add them together and push the result onto the stack.

Because of the way the original games VM is structured, there can be recursive calls to
the instruction interpreter and executor stages once a ``CALL``-instruction is encountered.


Symbol Data Storage
~~~~~~~~~~~~~~~~~~~

As many programming languages do, daedalus calls variables or functions *symbols* internally.
Symbols can have a name and a kind (e.g. *integer variable* or *external function*) so they
can be identified. Which symbol is valid to be used in which context is something the compiler
would check, but the daedalus VM needs to also adhere to the same rulse. For example,
it makes sense to ``CALL`` a function, but it makes no sense to ``CALL`` a string.

Other than having a kind, symbols will also have data associated with them. For an interger
symbol that would be the value, for example. A function symbol will also carry the address
of the function with it.

The following kinds of symbols exist:

- *Void*
- *Float*
- *Int*
- *String*
- *Class*
- *Func*
- *Instance*
- *Prototype*

.. warning:: Special care has to be taken as any symbol with the
  *Class-Variable* flag set could be part of a script object so the underlaying
  data could change as the *Current Instance* is set to a different object! See
  `Global Registers`_ for more information.
  

Symbolkind *Void*
^^^^^^^^^^^^^^^^^

The *Void* type is only used for the return type of functions/externals stored in
the symbol offset member. The symbols return flag should be present if not the
functions return type is any other than ``void``.

Else, *Void* is not expected to be used as symbol type (only for error handling and
internally at runtime for end-of-parameters in external definitions).

Symbolkind *Float*
^^^^^^^^^^^^^^^^^^

This kind of symbol will store one or more 32-bit floating point values. The arraylength
is comming from the .DAT-file.

Note that daedalus does not have actual support for floats, but they can be still used
as a parameter for externals.


Symbolkind *Int*
^^^^^^^^^^^^^^^^

This kind of symbol will store one or more 32-bit signed integer values. The arraylength
is comming from the .DAT-file.


Symbolkind *String*
^^^^^^^^^^^^^^^^^^^

This kind of symbol will store one or more string values. The arraylength
is comming from the .DAT-file.


Symbolkind *Class*
^^^^^^^^^^^^^^^^^^

In the original game, script classes were embedded into their native engine counterparts.
Embedded means, that the data of the script class was a substructure inside the native
class. The script code itself would get a raw native pointer to the native object
and an offset to the location where the data of the script class started.
Then it would just do raw memory access to modify that data.

Hence, symbols of this kind will store the offset of the script data inside the native
class. Since REGoth uses a safer approach, this is not used.


Symbolkind *Function*
^^^^^^^^^^^^^^^^^^^^^

Symbols of the *Function*-kind describe script functions which can be called by the VM
itself or by the script code. The symbol will hold the address of the function in the
instruction memory (byte code).

.. note:: If the *External*-flag is set, the address of the function would not point into the
   instruction memory but rather be a *raw native function pointer* into the native game
   code.


Symbolkind *Instance*
^^^^^^^^^^^^^^^^^^^^^

This kind of symbol stores a reference to a script object. This could be a character, an item,
a quest or others.

.. warning::
   This is not to be confused with an *Instance Function*.
   In Daedalus, an *Instance* is a function similar to a constructor of an object. Once a script
   object is created, its *Instance* function needs to be ran. For example, after creating a
   blank script object for an NPC, we can run an *Instance*-function to make the NPC
   whoever we want.
   
   An instance-function can also call arbitrary functions. It really is a usual script function
   with a fancy name.


Symbolkind *Prototype*
^^^^^^^^^^^^^^^^^^^^^^

In Daedalus, a *Prototype* is similar to an abstract class. It is like an *Instance*
that you cannot construct, but you can derive from it. Code put within the
*Prototypes* constructor will run before the *Instance* constructor so they is mostly
used for some general setup while an *Instance* sets more specialized parameters of
whatever it describes.

For example, ``NPC_Default`` is a prototype which sets up a default character. All other
Characters will derive from it and modify only what they need.
    

External Function Mapping
~~~~~~~~~~~~~~~~~~~~~~~~~

Functions called by script code can not only be other script functions but also
*native engine functions*, called *Externals*. This is used whenever a something
was too hard to implement in daedalus, not fast enough or simply not possible.
Most of the external functions however trigger some sort of game mechanics
related actions which are then handled by the native engine, for example letting a
character run to some location or adding a quest log entry.

The original game stored *raw native function addresses* within the .DAT-file so their
DaedalusVM could call directly into the native code. However, for better compatibility
between version, those values are scrapped and re-evaluated after loading the .DAT-File.

In REGoth, we just do the lookup of the native function address in a similar
fashion, by keeping a mapping of *External Symbol* to *Native Function* with the
VM which is generated after loading. Once the *Executor* encouters a
``CALL_EXTERNAL`` it can then look up which native function to call via the
symbol referenced within the instruction.


Global Registers
~~~~~~~~~~~~~~~~

Unlike a real processor, the Daedalus VM does not need registers for adding or
subtracting numbers as it can just use those of the host CPU.
However, there are a small number of specical registers controlling the executing of
script code:

 - *Program Counter* (*PC*)
 - *Current Instance*

Program Counter
^^^^^^^^^^^^^^^

The *Program Counter* register is just as one would expect: It points to the instruction which is
to be executed next within the instruciton memory. It is either increased as the program flow
continues or set to a completely different location after a ``JUMP`` or function call.

.. note:: There is no need to push it to the stack, since the stack of the host machine can be used.


Current Instance
^^^^^^^^^^^^^^^^

Within *Current Instance*, the game can set something similar to a *this-pointer*, which is used
by the *Instance* constructors.

It is usually set via the ``SET_INSTANCE``-instruction, which takes a script
symbol of the *Instance*-kind, which stores a reference to a script object.  The
*Current Instance* will then be set to the referenced script object.

All variables accessed which have the *Class-Variable*-flag set will then need
to look up their data values from the referenced script object.
