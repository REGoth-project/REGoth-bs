#include "DaedalusVMForGameWorld.hpp"
#include "DaedalusClassVarResolver.hpp"
#include <RTTI/RTTI_DaedalusVMForGameWorld.hpp>
#include <Scene/BsSceneObject.h>
#include <components/Character.hpp>
#include <components/CharacterEventQueue.hpp>
#include <components/GameClock.hpp>
#include <components/GameWorld.hpp>
#include <components/Item.hpp>
#include <components/VisualCharacter.hpp>

namespace REGoth
{
  namespace Scripting
  {
    DaedalusVMForGameWorld::DaedalusVMForGameWorld(HGameWorld gameWorld,
                                                   const bs::Vector<bs::UINT8>& datFileData)
        : DaedalusVM(datFileData)
        , mWorld(gameWorld)
    {
    }

    void DaedalusVMForGameWorld::fillSymbolStorage()
    {
      DaedalusVM::fillSymbolStorage();

      mHeroSymbol   = scriptSymbols().findIndexBySymbolName("HERO");
      mSelfSymbol   = scriptSymbols().findIndexBySymbolName("SELF");
      mOtherSymbol  = scriptSymbols().findIndexBySymbolName("OTHER");
      mVictimSymbol = scriptSymbols().findIndexBySymbolName("VICTIM");
      mItemSymbol   = scriptSymbols().findIndexBySymbolName("ITEM");
    }

    ScriptObjectHandle DaedalusVMForGameWorld::instanciateClass(const bs::String& className,
                                                                const bs::String& instanceName,
                                                                bs::HSceneObject mappedSceneObject)
    {
      SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(instanceName);

      return instanciateClass(className, symbol, mappedSceneObject);
    }

    ScriptObjectHandle DaedalusVMForGameWorld::instanciateClass(const bs::String& className,
                                                                SymbolIndex instanceSymbolIndex,
                                                                bs::HSceneObject mappedSceneObject)
    {
      SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(instanceSymbolIndex);

      return instanciateClass(className, symbol, mappedSceneObject);
    }

    ScriptObjectHandle DaedalusVMForGameWorld::instanciateClass(const bs::String& className,
                                                                SymbolInstance& instance,
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

      if (mappedSceneObject)
      {
        // Constructor might call an external and refer to the scene object, so map before doing
        // anything.
        mapping().map(obj, mappedSceneObject);
      }

      instance.instance = obj;

      ScriptObjectHandle oldCurrentInstance = mClassVarResolver->getCurrentInstance();
      ScriptObjectHandle oldSelf            = getInstance("SELF");

      setInstance("SELF", obj);
      mClassVarResolver->setCurrentInstance(obj);

      executeScriptFunction(instance.constructorAddress);

      mClassVarResolver->setCurrentInstance(oldCurrentInstance);
      setInstance("SELF", oldSelf);

      ScriptObject& objData = mScriptObjects.get(obj);
      // debugLogScriptObject(objData);

      return obj;
    }

    void DaedalusVMForGameWorld::runFunctionOnSelf(const bs::String& function, HCharacter self)
    {
      self->useAsSelf();
      executeScriptFunction(function);
    }

    bool DaedalusVMForGameWorld::runStateLoopFunction(SymbolIndex function, HCharacter self)
    {
      self->useAsSelf();

      const auto& functionSym = scriptSymbols().getSymbol<SymbolScriptFunction>(function);
      executeScriptFunction(functionSym.address);

      return popIntValue() != 0;
    }

    void DaedalusVMForGameWorld::runFunctionOnSelf(SymbolIndex function, HCharacter self)
    {
      self->useAsSelf();

      const auto& functionSym = scriptSymbols().getSymbol<SymbolScriptFunction>(function);
      executeScriptFunction(functionSym.address);
    }

    void DaedalusVMForGameWorld::setHero(ScriptObjectHandle hero)
    {
      setInstance(mHeroSymbol, hero);
    }

    ScriptObjectHandle DaedalusVMForGameWorld::heroInstance()
    {
      return getInstance(mHeroSymbol);
    }

    ScriptObjectHandle DaedalusVMForGameWorld::victimInstance() const
    {
      return getInstance(mVictimSymbol);
    }

    HCharacter DaedalusVMForGameWorld::victim() const
    {
      return getInstanceCharacter(mVictimSymbol);
    }

    void DaedalusVMForGameWorld::setVictim(ScriptObjectHandle victim)
    {
      setInstance(mVictimSymbol, victim);
    }

    ScriptObjectHandle DaedalusVMForGameWorld::itemInstance() const
    {
      return getInstance(mItemSymbol);
    }

    HItem DaedalusVMForGameWorld::item() const
    {
      return getInstanceItem(mItemSymbol);
    }

    void DaedalusVMForGameWorld::setItem(ScriptObjectHandle item)
    {
      setInstance(mItemSymbol, item);
    }

    ScriptObjectHandle DaedalusVMForGameWorld::otherInstance() const
    {
      return getInstance(mOtherSymbol);
    }

    HCharacter DaedalusVMForGameWorld::other() const
    {
      return getInstanceCharacter(mOtherSymbol);
    }

    void DaedalusVMForGameWorld::setOther(ScriptObjectHandle other)
    {
      setInstance(mOtherSymbol, other);
    }

    ScriptObjectHandle DaedalusVMForGameWorld::selfInstance() const
    {
      return getInstance(mSelfSymbol);
    }

    HCharacter DaedalusVMForGameWorld::self() const
    {
      return getInstanceCharacter(mSelfSymbol);
    }

    void DaedalusVMForGameWorld::setSelf(ScriptObjectHandle self)
    {
      setInstance(mSelfSymbol, self);
    }

    void DaedalusVMForGameWorld::setInstance(SymbolIndex instance, ScriptObjectHandle scriptObject)
    {
      SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(instance);

      symbol.instance = scriptObject;
    }

    void DaedalusVMForGameWorld::setInstance(const bs::String& instance,
                                             ScriptObjectHandle scriptObject)
    {
      SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(instance);

      symbol.instance = scriptObject;
    }

    ScriptObjectHandle DaedalusVMForGameWorld::getInstance(const bs::String& instance) const
    {
      const SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(instance);

      return symbol.instance;
    }

    ScriptObjectHandle DaedalusVMForGameWorld::getInstance(SymbolIndex symbolIndex) const
    {
      const SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(symbolIndex);

      return symbol.instance;
    }

    HCharacter DaedalusVMForGameWorld::getInstanceCharacter(const bs::String& instance) const
    {
      const SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(instance);

      bs::HSceneObject characterSO = mappingConst().getMappedSceneObject(symbol.instance);

      HCharacter character = characterSO->getComponent<Character>();

      if (!character)
      {
        REGOTH_THROW(InvalidParametersException,
                     "Expected to be passed a Character instance, but the scene object does "
                     "not have a character-component!");
      }

      return character;
    }

    HCharacter DaedalusVMForGameWorld::getInstanceCharacter(SymbolIndex symbolIndex) const
    {
      const SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(symbolIndex);

      bs::HSceneObject characterSO = mappingConst().getMappedSceneObject(symbol.instance);

      HCharacter character = characterSO->getComponent<Character>();

      if (!character)
      {
        REGOTH_THROW(InvalidParametersException,
                     "Expected to be passed a Character instance, but the scene object does "
                     "not have a character-component!");
      }

      return character;
    }

    HItem DaedalusVMForGameWorld::getInstanceItem(const bs::String& instance) const
    {
      const SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(instance);

      bs::HSceneObject itemSO = mappingConst().getMappedSceneObject(symbol.instance);

      HItem item = itemSO->getComponent<Item>();

      if (!item)
      {
        REGOTH_THROW(InvalidParametersException,
                     "Expected to be passed a Item instance, but the scene object does "
                     "not have a item-component!");
      }

      return item;
    }

    HItem DaedalusVMForGameWorld::getInstanceItem(SymbolIndex symbolIndex) const
    {
      const SymbolInstance& symbol = mScriptSymbols.getSymbol<SymbolInstance>(symbolIndex);

      bs::HSceneObject itemSO = mappingConst().getMappedSceneObject(symbol.instance);

      HItem item = itemSO->getComponent<Item>();

      if (!item)
      {
        REGOTH_THROW(InvalidParametersException,
                     "Expected to be passed a Item instance, but the scene object does "
                     "not have a item-component!");
      }

      return item;
    }

    HCharacter DaedalusVMForGameWorld::popCharacterInstance()
    {
      ScriptObjectHandle scriptObject = popInstanceScriptObject();

      if (scriptObject == SCRIPT_OBJECT_HANDLE_INVALID)
      {
        return {};
      }

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

    HItem DaedalusVMForGameWorld::popItemInstance()
    {
      ScriptObjectHandle scriptObject = popInstanceScriptObject();

      if (scriptObject == SCRIPT_OBJECT_HANDLE_INVALID)
      {
        return {};
      }

      bs::HSceneObject itemSO = mapping().getMappedSceneObject(scriptObject);

      HItem item = itemSO->getComponent<Item>();

      if (!item)
      {
        REGOTH_THROW(InvalidParametersException,
                     "Expected to find a item instance on the stack, but the scene object does "
                     "not have a item-component!");
      }

      return item;
    }

    void DaedalusVMForGameWorld::initializeWorld(const bs::String& worldName)
    {
      // Some scripts already refer to the hero, so make sure that has been set
      if (getInstance("HERO") == SCRIPT_OBJECT_HANDLE_INVALID)
      {
        REGOTH_THROW(InvalidStateException, "Hero-instance must be set to call world init scripts!");
      }

      // Some G1-Scripts refer to SELF during init while doing debug-output.
      // I can only assume they mean the hero.
      setInstance("SELF", getInstance("HERO"));

      // FIXME: Do STARTUP_* only on first load?
      executeScriptFunction("STARTUP_" + worldName);

      executeScriptFunction("INIT_" + worldName);
    }

    void DaedalusVMForGameWorld::registerAllExternals()
    {
      using This = DaedalusVMForGameWorld;

      registerExternal("PRINT", (externalCallback)&This::external_Print);
      registerExternal("HLP_RANDOM", (externalCallback)&This::external_HLP_Random);
      registerExternal("HLP_GETNPC", (externalCallback)&This::external_HLP_GetNpc);
      registerExternal("HLP_ISVALIDNPC", (externalCallback)&This::external_HLP_IsValidNpc);
      registerExternal("HLP_ISVALIDITEM", (externalCallback)&This::external_HLP_IsValidItem);
      registerExternal("INTTOSTRING", (externalCallback)&This::external_IntToString);
      registerExternal("INTTOFLOAT", (externalCallback)&This::external_IntToFloat);
      registerExternal("FLOATTOINT", (externalCallback)&This::external_FloatToInt);
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
      registerExternal("WLD_GETDAY", (externalCallback)&This::external_WLD_GetDay);
      registerExternal("WLD_ISTIME", (externalCallback)&This::external_WLD_IsTime);
      registerExternal("WLD_SETTIME", (externalCallback)&This::external_WLD_SetTime);
      registerExternal("TA_MIN", (externalCallback)&This::external_TA_Min);
      registerExternal("NPC_EXCHANGEROUTINE", (externalCallback)&This::external_NPC_ExchangeRoutine);
      registerExternal("AI_GOTOWP", (externalCallback)&This::external_AI_GotoWaypoint);
    }

    void DaedalusVMForGameWorld::external_Print()
    {
      bs::gDebug().logDebug("[ScriptVMInterface] [Print] " + popStringValue());
    }

    void DaedalusVMForGameWorld::external_HLP_Random()
    {
      mStack.pushInt(rand() % popIntValue());
    }

    void DaedalusVMForGameWorld::external_HLP_GetNpc()
    {
      bs::INT32 symbolIndex = popIntValue();

      mStack.pushInstance((SymbolIndex)symbolIndex);
    }

    void DaedalusVMForGameWorld::external_HLP_IsValidNpc()
    {
      HCharacter character = popCharacterInstance();

      mStack.pushInt(!!character ? 1 : 0);
    }

    void DaedalusVMForGameWorld::external_HLP_IsValidItem()
    {
      HItem item = popItemInstance();

      mStack.pushInt(!!item ? 1 : 0);
    }

    void DaedalusVMForGameWorld::external_IntToString()
    {
     mStack.pushString(bs::toString(popIntValue()));
    }

    void DaedalusVMForGameWorld::external_IntToFloat()
    {
      mStack.pushFloat((float)popIntValue());
    }

    void DaedalusVMForGameWorld::external_FloatToInt()
    {
      mStack.pushInt((bs::INT32)popFloatValue());
    }

    void DaedalusVMForGameWorld::external_ConcatStrings()
    {
      bs::String b = popStringValue();
      bs::String a = popStringValue();

      mStack.pushString(a + b);
    }

    void DaedalusVMForGameWorld::external_WLD_InsertItem()
    {
      bs::String spawnpoint = popStringValue();
      SymbolIndex instance  = popIntValue();

      mWorld->insertItem(mScriptSymbols.getSymbolName(instance), spawnpoint);
    }

    void DaedalusVMForGameWorld::external_WLD_InsertNpc()
    {
      bs::String waypoint  = popStringValue();
      SymbolIndex instance = popIntValue();

      mWorld->insertCharacter(mScriptSymbols.getSymbolName(instance), waypoint);
    }

    void DaedalusVMForGameWorld::external_WLD_GetDay()
    {
      bs::INT32 day = mWorld->gameclock()->getDay();

      mStack.pushInt(day);
    }

    void DaedalusVMForGameWorld::external_WLD_IsTime()
    {
      bs::INT32 min2  = popIntValue();
      bs::INT32 hour2 = popIntValue();
      bs::INT32 min1  = popIntValue();
      bs::INT32 hour1 = popIntValue();

      bool test = mWorld->gameclock()->isTime(hour1, min1, hour2, min2);

      mStack.pushInt(test ? 1 : 0);
    }

    void DaedalusVMForGameWorld::external_WLD_SetTime()
    {
      bs::INT32 min  = popIntValue();
      bs::INT32 hour = popIntValue();

      mWorld->gameclock()->setTime(hour, min);
    }

    void DaedalusVMForGameWorld::external_NPC_IsPlayer()
    {
      HCharacter character = popCharacterInstance();

      mStack.pushInt(character->isPlayer() ? 1 : 0);
    }

    void DaedalusVMForGameWorld::external_NPC_SetTalentSkill()
    {
      bs::INT32 skill      = popIntValue();
      bs::INT32 talent     = popIntValue();
      HCharacter character = popCharacterInstance();

      bs::gDebug().logWarning("[External] Using external stub: NPC_SetTalentSkill");
    }

    void DaedalusVMForGameWorld::external_NPC_EquipItem()
    {
      bs::String instance  = popStringValue();
      HCharacter character = popCharacterInstance();

      character->equipItem(instance);
    }
    void DaedalusVMForGameWorld::external_NPC_CreateInventoryItems()
    {
      bs::INT32 num        = popIntValue();
      bs::String instance  = popStringValue();
      HCharacter character = popCharacterInstance();

      character->createInventoryItem(instance, num);
    }

    void DaedalusVMForGameWorld::external_NPC_CreateInventoryItem()
    {
      bs::String instance  = popStringValue();
      HCharacter character = popCharacterInstance();

      character->createInventoryItem(instance, 1);
    }

    void DaedalusVMForGameWorld::external_MDL_SetVisual()
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

    void DaedalusVMForGameWorld::external_MDL_SetVisualBody()
    {
      bs::INT32 armorInstance = popIntValue();

      bs::INT32 teethTexIndex = popIntValue();
      bs::INT32 headTexIndex  = popIntValue();
      bs::String headMesh     = popStringValue();

      bs::INT32 bodyTexColor = popIntValue();
      bs::INT32 bodyTexIndex = popIntValue();
      bs::String bodyMesh    = popStringValue();

      // If an armor is set here, we need to replace the body mesh from the input parameters with the
      // visual settings from inside the armor instance
      if (armorInstance != -1)
      {
        // TODO: Original Gothic adds the Armor straight to the inventory. We just create a
        // temporary instance to extract the visual information from it
        ScriptObjectHandle armorSObj = getInstance(armorInstance);

        if (!armorSObj)
        {
          armorSObj = instanciateClass("C_ITEM", armorInstance, {});
        }

        ScriptObject& armor = mScriptObjects.get(armorSObj);

        bodyMesh = armor.stringValue("VISUAL_CHANGE");
      }

      HCharacter character = popCharacterInstance();

      HVisualCharacter characterVisual = character->SO()->getComponent<VisualCharacter>();

      bs::StringUtil::toUpperCase(bodyMesh);
      characterVisual->setBodyMesh(bodyMesh);

      bs::StringUtil::toUpperCase(headMesh);
      characterVisual->setHeadMesh(headMesh);
    }

    void DaedalusVMForGameWorld::external_AI_GotoWaypoint()
    {
      bs::String waypoint = popStringValue();
      HCharacter self     = popCharacterInstance();

      bs::StringUtil::toUpperCase(waypoint);

      auto eventQueue = self->SO()->getComponent<CharacterEventQueue>();

      eventQueue->pushGotoObject(mWorld->findObjectByName(waypoint));
    }

    void DaedalusVMForGameWorld::external_TA_Min()
    {
      bs::String waypoint = popStringValue();
      SymbolIndex action  = popIntValue(); // This does not push onto the function stack for some reason
      bs::INT32 stop_m    = popIntValue();
      bs::INT32 stop_h    = popIntValue();
      bs::INT32 start_m   = popIntValue();
      bs::INT32 start_h   = popIntValue();
      HCharacter self     = popCharacterInstance();

      bs::StringUtil::toUpperCase(waypoint);

      AI::ScriptState::RoutineTask task;
      task.hoursStart     = start_h;
      task.hoursEnd       = stop_h;

      task.minutesStart   = start_m;
      task.minutesEnd     = stop_m;

      task.waypoint       = waypoint;

      if (action != SYMBOL_INDEX_INVALID)
      {
        task.scriptFunction = scriptSymbols().getSymbolName(action);
      }

      auto eventQueue = self->SO()->getComponent<CharacterEventQueue>();

      eventQueue->insertRoutineTask(task);
    }

    void DaedalusVMForGameWorld::external_NPC_ExchangeRoutine()
    {
      bs::String routineName = popStringValue();
      HCharacter self        = popCharacterInstance();

      auto eventQueue = self->SO()->getComponent<CharacterEventQueue>();

      bs::StringUtil::toUpperCase(routineName);

      self->setDailyRoutine(routineName);

      eventQueue->reinitRoutine();
    }

    void DaedalusVMForGameWorld::script_PrintPlus(const bs::String& text)
    {
      mStack.pushString(text);
      executeScriptFunction("PrintPlus");
    }

    REGOTH_DEFINE_RTTI(DaedalusVMForGameWorld)
  }  // namespace Scripting
}  // namespace REGoth
