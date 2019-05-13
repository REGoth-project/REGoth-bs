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
      ScriptObjectHandle instanciateClass(const bs::String& className,
                                          SymbolInstance& instance,
                                          bs::HSceneObject mappedSceneObject);

      void initializeWorld(const bs::String& worldName) override;
      void setHero(ScriptObjectHandle hero) override;
      ScriptObjectHandle getHero() override;

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

      void registerAllExternals() override;

    protected:
      /**
       * Handle to the game world this is used in
       */
      HGameWorld mWorld;

    public:
      REGOTH_DECLARE_RTTI(DaedalusVMForGameWorld);

    public:                                // FIXME: RTTI protected
      DaedalusVMForGameWorld() = default;  // For RTTI
    };
  }  // namespace Scripting
}  // namespace REGoth
