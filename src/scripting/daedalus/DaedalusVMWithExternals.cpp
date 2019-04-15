#include "DaedalusVMWithExternals.hpp"
#include "DaedalusClassVarResolver.hpp"
#include <Scene/BsSceneObject.h>
#include <components/Character.hpp>
#include <components/VisualCharacter.hpp>
#include <world/GameWorld.hpp>

namespace REGoth
{
  namespace Scripting
  {
    DaedalusVMWithExternals::DaedalusVMWithExternals(const Daedalus::DATFile& datFile)
        : DaedalusVM(datFile)
    {
    }

    ScriptObjectHandle DaedalusVMWithExternals::instanciateClass(const bs::String& className,
                                                                 const bs::String& instanceName,
                                                                 bs::HSceneObject mappedSceneObject)
    {
      // To instanciate a class the following has to happen:
      //
      //  1. Create a blank object of that class,
      //  2. Save `self` and *Current Instance*,
      //  3. Assign the new object to `self` and *Current Instance*,
      //  4. Run the instance constructor
      //  5. Restore `self` and *Current Instance*,
      //  6. Assign the newly created object to the instances symbol
      //
      // *Current Instance* must be set so the class-members are resolved correctly.
      // `self` must be set so that the constructor can refer to the object being created
      // in function calls.
      //
      // The last created instance is also assigned to the instance symbol in step 6 so that
      // it can be referred to by name inside the scripts. Though, this only makes sense for
      // instances where there is usually only one instance active.
      //
      // Since the constructor *can* call externals which refer to the scene object, we need
      // to also map the objects *before* executing the constructor.

      ScriptObjectHandle obj = instanciateBlankObjectOfClass(className);
      SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(instanceName);

      // Constructor might call an external and refer to the scene object, so map before doing
      // anything.
      mapping().map(obj, mappedSceneObject);
      symbol.instance = obj;

      ScriptObjectHandle oldCurrentInstance = mClassVarResolver->getCurrentInstance();
      ScriptObjectHandle oldSelf            = getInstance("SELF");

      setInstance("SELF", obj);
      mClassVarResolver->setCurrentInstance(obj);

      executeScriptFunction(symbol.constructorAddress);

      mClassVarResolver->setCurrentInstance(oldCurrentInstance);
      setInstance("SELF", oldSelf);

      ScriptObject& objData = mScriptObjects.get(obj);
      // debugLogScriptObject(objData);

      return obj;
    }

    void DaedalusVMWithExternals::setHero(ScriptObjectHandle hero)
    {
      setInstance("HERO", hero);
    }

    void DaedalusVMWithExternals::setInstance(const bs::String& instance,
                                              ScriptObjectHandle scriptObject)
    {
      SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(instance);

      symbol.instance = scriptObject;
    }

    ScriptObjectHandle DaedalusVMWithExternals::getInstance(const bs::String& instance) const
    {
      const SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(instance);

      return symbol.instance;
    }

    HCharacter DaedalusVMWithExternals::popCharacterInstance()
    {
      ScriptObjectHandle scriptObject = popInstanceScriptObject();

      bs::HSceneObject characterSO = mapping().getMappedSceneObject(scriptObject);

      HCharacter character = characterSO->getComponent<Character>();

      if (!character)
      {
        REGOTH_THROW(InvalidParametersException,
                     "Expected to find a character instance on the stack, but the scene object does "
                     "not have a character-component!");
      }

      return character;
    }

    void DaedalusVMWithExternals::initializeWorld(const bs::String& worldName)
    {
      // FIXME: Do STARTUP_* only on first load?
      executeScriptFunction("STARTUP_" + worldName);

      executeScriptFunction("INIT_" + worldName);
    }

    void DaedalusVMWithExternals::registerAllExternals()
    {
      using This = DaedalusVMWithExternals;

      registerExternal("PRINT", (externalCallback)&This::external_Print);
      registerExternal("HLP_RANDOM", (externalCallback)&This::external_HLP_Random);
      registerExternal("HLP_GETNPC", (externalCallback)&This::external_HLP_GetNpc);
      registerExternal("INTTOSTRING", (externalCallback)&This::external_IntToString);
      registerExternal("NPC_ISPLAYER", (externalCallback)&This::external_NPC_IsPlayer);
      registerExternal("WLD_INSERTNPC", (externalCallback)&This::external_WLD_InsertNpc);
      registerExternal("CONCATSTRINGS", (externalCallback)&This::external_ConcatStrings);
      registerExternal("WLD_INSERTITEM", (externalCallback)&This::external_WLD_InsertItem);
      registerExternal("NPC_SETTALENTSKILL", (externalCallback)&This::external_NPC_SetTalentSkill);
      registerExternal("EQUIPITEM", (externalCallback)&This::external_NPC_EquipItem);
      registerExternal("CREATEINVITEMS", (externalCallback)&This::external_NPC_CreateInventoryItems);
      registerExternal("CREATEINVITEM", (externalCallback)&This::external_NPC_CreateInventoryItem);
      registerExternal("MDL_SETVISUAL", (externalCallback)&This::external_MDL_SetVisual);
      registerExternal("MDL_SETVISUALBODY", (externalCallback)&This::external_MDL_SetVisualBody);
    }

    void DaedalusVMWithExternals::external_Print()
    {
      print(popStringValue());
    }

    void DaedalusVMWithExternals::external_HLP_Random()
    {
      mStack.pushInt(HLP_Random(popIntValue()));
    }

    void DaedalusVMWithExternals::external_HLP_GetNpc()
    {
      bs::INT32 symbolIndex = popIntValue();

      mStack.pushInstance((SymbolIndex)symbolIndex);
    }

    void DaedalusVMWithExternals::external_IntToString()
    {
      mStack.pushString(IntToString(popIntValue()));
    }

    void DaedalusVMWithExternals::external_ConcatStrings()
    {
      bs::String b = popStringValue();
      bs::String a = popStringValue();

      mStack.pushString(ConcatStrings(a, b));
    }

    void DaedalusVMWithExternals::external_WLD_InsertItem()
    {
      bs::String spawnpoint = popStringValue();
      SymbolIndex instance  = popIntValue();

      gWorld().insertItem(mScriptSymbols.getSymbolName(instance), spawnpoint);
    }

    void DaedalusVMWithExternals::external_WLD_InsertNpc()
    {
      bs::String waypoint  = popStringValue();
      SymbolIndex instance = popIntValue();

      gWorld().insertCharacter(mScriptSymbols.getSymbolName(instance), waypoint);
    }

    void DaedalusVMWithExternals::external_NPC_IsPlayer()
    {
      HCharacter character = popCharacterInstance();

      mStack.pushInt(character->isPlayer() ? 1 : 0);
    }

    void DaedalusVMWithExternals::external_NPC_SetTalentSkill()
    {
      bs::INT32 skill      = popIntValue();
      bs::INT32 talent     = popIntValue();
      HCharacter character = popCharacterInstance();

      bs::gDebug().logWarning("[External] Using external stub: NPC_SetTalentSkill");
    }

    void DaedalusVMWithExternals::external_NPC_EquipItem()
    {
      bs::String instance  = popStringValue();
      HCharacter character = popCharacterInstance();

      character->equipItem(instance);
    }
    void DaedalusVMWithExternals::external_NPC_CreateInventoryItems()
    {
      bs::INT32 num        = popIntValue();
      bs::String instance  = popStringValue();
      HCharacter character = popCharacterInstance();

      character->createInventoryItem(instance, num);
    }

    void DaedalusVMWithExternals::external_NPC_CreateInventoryItem()
    {
      bs::String instance  = popStringValue();
      HCharacter character = popCharacterInstance();

      character->createInventoryItem(instance, 1);
    }

    void DaedalusVMWithExternals::external_MDL_SetVisual()
    {
      bs::String visual    = popStringValue();
      HCharacter character = popCharacterInstance();

      HVisualCharacter characterVisual;

      // if (!character->SO()->hasComponent<VisualCharacter>())
      // {
      // characterVisual = character->SO()->addComponent<VisualCharacter>();
      // }
      // else
      // {
      characterVisual = character->SO()->getComponent<VisualCharacter>();
      // }

      bs::StringUtil::toUpperCase(visual);
      characterVisual->setVisual(visual);
    }

    void DaedalusVMWithExternals::external_MDL_SetVisualBody()
    {
      bs::INT32 armorInstance = popIntValue();

      bs::INT32 teethTexIndex = popIntValue();
      bs::INT32 headTexIndex  = popIntValue();
      bs::String headMesh     = popStringValue();

      bs::INT32 bodyTexColor  = popIntValue();
      bs::INT32 bodyTexIndex  = popIntValue();
      bs::String bodyMesh     = popStringValue();

      HCharacter character = popCharacterInstance();

      HVisualCharacter characterVisual = character->SO()->getComponent<VisualCharacter>();

      bs::StringUtil::toUpperCase(bodyMesh);
      characterVisual->setBodyMesh(bodyMesh);

      bs::StringUtil::toUpperCase(headMesh);
      bs::gDebug().logDebug("Headmesh: " + headMesh);
      characterVisual->setHeadMesh(headMesh);
    }

    void DaedalusVMWithExternals::script_PrintPlus(const bs::String& text)
    {
      mStack.pushString(text);
      executeScriptFunction("PrintPlus");
    }
  }  // namespace Scripting
}  // namespace REGoth
