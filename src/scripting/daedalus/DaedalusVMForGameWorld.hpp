/**\file
 */
#pragma once
#include "REGothDaedalusVM.hpp"
#include <BsPrerequisites.h>

namespace REGoth
{
  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  class Item;
  using HItem = bs::GameObjectHandle<Item>;

  namespace Scripting
  {
    /**
     * DaedalusVM implementing the externals needed for GOTHIC.DAT.
     */
    class DaedalusVMForGameWorld : public DaedalusVM
    {
    public:
      DaedalusVMForGameWorld(HGameWorld gameWorld, const bs::Vector<bs::UINT8>& datFileData);

      /**
       * Initializes the ScriptVM. To be called after the object is constructed.
       */
      virtual void initialize();

      ScriptObjectHandle instanciateClass(const bs::String& className,
                                          const bs::String& instanceName,
                                          bs::HSceneObject mappedSceneObject) override;
      ScriptObjectHandle instanciateClass(const bs::String& className,
                                          SymbolIndex instanceSymbolIndex,
                                          bs::HSceneObject mappedSceneObject);
      ScriptObjectHandle instanciateClass(const bs::String& className, SymbolInstance& instance,
                                          bs::HSceneObject mappedSceneObject);

      /**
       * Runs a simple function without return type or parameters. But sets the
       * global self to the given character.
       *
       * @param  function  Function to call, e.g. `B_SOMETHING`.
       * @param  self      Character to set `self` to.
       */
      void runFunctionOnSelf(const bs::String& function, HCharacter self);
      void runFunctionOnSelf(SymbolIndex function, HCharacter self);

      /**
       * Calls a function used during the LOOP-Part of the script states. e.g. `ZS_TALK_LOOP`.
       * See AI::ScriptState for more information.
       *
       * @note  This function will clean the stack! Some script functions don't push a return value
       *        and so we would get whatever was on the stack before. Since this is only called from
       *        engine code, it's okay to throw away the whole script stack here.
       *
       * @return Whether the State is done.
       */
      bool runStateLoopFunction(SymbolIndex function, HCharacter self);

      /**
       * Wrapper to call the function set in `C_INFO.condition` to check whether a dialogue line
       * should be displayed to the user in the UI.
       *
       * @note  This function will clean the stack! Some script functions don't push a return value
       *        and so we would get whatever was on the stack before. Since this is only called from
       *        engine code, it's okay to throw away the whole script stack here.
       *
       * @return True, if the dialogue line should be visible.
       */
      bool runInfoConditionFunction(SymbolIndex function, HCharacter self, HCharacter other);

      /**
       * Wrapper to call the function set in `C_INFO.information`.
       */
      void runInfoFunction(SymbolIndex function, HCharacter self, HCharacter other);

      void initializeWorld(const bs::String& worldName) override;

      void setHero(ScriptObjectHandle hero);
      ScriptObjectHandle heroInstance();

      /**
       * @return The script object in the instance `VICTIM`.
       */
      ScriptObjectHandle victimInstance() const;
      HCharacter victim() const;
      void setVictim(ScriptObjectHandle victim);

      /**
       * @return The script object in the instance `ITEM`.
       */
      ScriptObjectHandle itemInstance() const;
      HItem item() const;
      void setItem(ScriptObjectHandle item);

      /**
       * @return The script object in the instance `OTHER`.
       */
      ScriptObjectHandle otherInstance() const;
      HCharacter other() const;
      void setOther(ScriptObjectHandle other);

      /**
       * @return The script object in the instance `SELF`.
       */
      ScriptObjectHandle selfInstance() const;
      HCharacter self() const;
      void setSelf(ScriptObjectHandle self);

      /**
       * @return All *Information*-Instances meant for the given NPC. See
       * createAllInformationInstances() for more information.
       */
      const bs::Vector<ScriptObjectHandle>& allInfosOfNpc(const bs::String& instanceName) const;

    protected:

      /**
       * Fills mAllInformationInstances. This is done here at one place because otherwise
       * every single created NPC would need to loop through all symbols every time, find
       * the `C_INFO` instances, create instances, check whether they are for the correct
       * npc and so on. This would be rather inefficient, so we just keep a list of all
       * *Information*-instances here.
       */
      void createAllInformationInstances();

      /**
       * Does popInstance() and resolves the Character-component.
       *
       * Throws if the instance on the stack is not a character.
       *
       * @return Character from the stack. Empty handle if the instance
       *         wasn't referencing something.
       */
      HCharacter popCharacterInstance();

      /**
       * Does popInstance() and resolves the Item-component.
       *
       * Throws if the instance on the stack is not a item.
       *
       * @return Item from the stack. Empty handle if the instance
       *         wasn't referencing something.
       */
      HItem popItemInstance();

      /**
       * Sets the given instance to the given script object.
       *
       * Throws if the instance does not exist.
       *
       * @param  instance      Instance to set, e.g. `self`.
       * @oaram  scriptObject  Script object to assign.
       */
      void setInstance(const bs::String& instance, ScriptObjectHandle scriptObject);
      void setInstance(SymbolIndex instance, ScriptObjectHandle scriptObject);

      /**
       * Looks up what the given instance has assigned to.
       *
       * Throws if the instance does not exist.
       *
       * @param  instance  Instance to query, e.g. `self`.
       *
       * @return Handle the instance was set to. Might be invalid!
       */
      ScriptObjectHandle getInstance(const bs::String& instance) const;
      ScriptObjectHandle getInstance(SymbolIndex symbolIndex) const;

      /**
       * Looks up what the given instance has assigned to and casts it to a character.
       *
       * Throws if the instance does not exist.
       *
       * @param  instance  Instance to query, e.g. `self`.
       *
       * @return Handle the instance was set to. Might be invalid!
       */
      HCharacter getInstanceCharacter(const bs::String& instance) const;
      HCharacter getInstanceCharacter(SymbolIndex symbolIndex) const;
      HItem getInstanceItem(const bs::String& instance) const;
      HItem getInstanceItem(SymbolIndex symbolIndex) const;

      void script_PrintPlus(const bs::String& text);

      void external_Print();
      void external_HLP_Random();
      void external_HLP_GetNpc();
      void external_HLP_IsValidNpc();
      void external_HLP_IsValidItem();
      void external_IntToString();
      void external_IntToFloat();
      void external_FloatToInt();
      void external_ConcatStrings();
      void external_WLD_InsertItem();
      void external_WLD_InsertNpc();
      void external_WLD_GetDay();
      void external_WLD_IsTime();
      void external_WLD_SetTime();
      void external_NPC_IsPlayer();
      void external_NPC_SetTalentSkill();
      void external_NPC_EquipItem();
      void external_NPC_CreateInventoryItems();
      void external_NPC_CreateInventoryItem();
      void external_MDL_SetVisual();
      void external_MDL_SetVisualBody();
      void external_AI_GotoWaypoint();
      void external_AI_GotoFreepoint();
      void external_AI_GotoNextFreepoint();
      void external_AI_GotoNpc();
      void external_TA_Min();
      void external_NPC_ExchangeRoutine();
      void external_AI_SetWalkMode();
      void external_AI_Wait();
      void external_AI_StartState();
      void external_AI_PlayAnimation();
      void external_Npc_GetNearestWP();
      void external_Npc_GetNextWP();
      void external_Npc_GetDistToWP();
      void external_Npc_GetDistToNpc();
      void external_Npc_GetDistToItem();
      void external_Npc_GetDistToPlayer();
      void external_Npc_IsNear();
      void external_Npc_SetToFistMode();
      void external_Npc_KnowsInfo();
      void external_InfoManager_HasFinished();
      void external_AI_ProcessInfos();
      void external_AI_StopProcessInfos();

      void fillSymbolStorage() override;
      void registerAllExternals() override;

    protected:
      /** Handle to the game world this is used in */
      HGameWorld mWorld;

      /** Quick access to often used symbols */
      SymbolIndex mHeroSymbol   = SYMBOL_INDEX_INVALID;
      SymbolIndex mSelfSymbol   = SYMBOL_INDEX_INVALID;
      SymbolIndex mOtherSymbol  = SYMBOL_INDEX_INVALID;
      SymbolIndex mVictimSymbol = SYMBOL_INDEX_INVALID;
      SymbolIndex mItemSymbol   = SYMBOL_INDEX_INVALID;

      /** Cache of all information instances for all NPCs */
      bs::Map<SymbolIndex, bs::Vector<ScriptObjectHandle>> mInformationInstancesByNpcs;

    public:
      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(DaedalusVMForGameWorld);

    protected:
      DaedalusVMForGameWorld() = default;  // For RTTI
    };
  }  // namespace Scripting
}  // namespace REGoth
