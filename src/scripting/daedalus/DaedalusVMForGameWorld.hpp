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
      DaedalusVMForGameWorld(HGameWorld gameWorld, const Daedalus::DATFile& datFile);

      ScriptObjectHandle instanciateClass(const bs::String& className,
                                          const bs::String& instanceName,
                                          bs::HSceneObject mappedSceneObject) override;
      ScriptObjectHandle instanciateClass(const bs::String& className,
                                          SymbolIndex instanceSymbolIndex,
                                          bs::HSceneObject mappedSceneObject);
      ScriptObjectHandle instanciateClass(const bs::String& className, SymbolInstance& instance,
                                          bs::HSceneObject mappedSceneObject);

      void initializeWorld(const bs::String& worldName) override;

      void setHero(ScriptObjectHandle hero);
      ScriptObjectHandle heroInstance();

      /**
       * @return The script object in the instance `VICTIM`.
       */
      ScriptObjectHandle victimInstance() const;
      HCharacter victim() const;

      /**
       * @return The script object in the instance `ITEM`.
       */
      ScriptObjectHandle itemInstance() const;
      HItem item() const;

      /**
       * @return The script object in the instance `OTHER`.
       */
      ScriptObjectHandle otherInstance() const;
      HCharacter other() const;

      /**
       * @return The script object in the instance `SELF`.
       */
      ScriptObjectHandle selfInstance() const;
      HCharacter self() const;

    protected:
      /**
       * Does popInstance() and resolves the Character-component.
       *
       * Throws if the instance on the stack is not a character.
       *
       * @return Character from the stack.
       */
      HCharacter popCharacterInstance();

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
      void external_IntToString();
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

      void fillSymbolStorage() override;
      void registerAllExternals() override;

    protected:
      /**
       * Handle to the game world this is used in
       */
      HGameWorld mWorld;

      // Quick access to often used symbols
      SymbolIndex mHeroSymbol   = SYMBOL_INDEX_INVALID;
      SymbolIndex mSelfSymbol   = SYMBOL_INDEX_INVALID;
      SymbolIndex mOtherSymbol  = SYMBOL_INDEX_INVALID;
      SymbolIndex mVictimSymbol = SYMBOL_INDEX_INVALID;
      SymbolIndex mItemSymbol   = SYMBOL_INDEX_INVALID;

    public:
      REGOTH_DECLARE_RTTI_FOR_REFLECTABLE(DaedalusVMForGameWorld);

    protected:
      DaedalusVMForGameWorld() = default;  // For RTTI
    };
  }  // namespace Scripting
}  // namespace REGoth
