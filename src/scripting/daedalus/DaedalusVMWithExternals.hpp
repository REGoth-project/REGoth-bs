/**\file
 */
#pragma once
#include "REGothDaedalusVM.hpp"
#include <BsPrerequisites.h>

namespace REGoth
{
  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  namespace Scripting
  {
    class IDaedalusScriptInterface
    {
    public:
      virtual void script_PrintPlus(const bs::String& text) = 0;
    };

    class IDaedalusScriptExternals
    {
    protected:
      virtual void print(const bs::String& text)  = 0;
      virtual bs::INT32 HLP_Random(bs::INT32 max) = 0;
      // virtual bool HLP_StringCompare(const bs::String& a, const bs::String& b)           = 0;
      virtual bs::String IntToString(bs::INT32 i) = 0;
      // virtual bs::INT32 FloatToInt(float f)                                              = 0;
      // virtual float IntToFloat(bs::INT32 i)                                              = 0;
      virtual bs::String ConcatStrings(const bs::String& a, const bs::String& b) = 0;
      // virtual void MDL_SetVisual(bs::HSceneObject characterSO, const bs::String& visual) = 0;

      struct VisualBody
      {
        bs::String body;
        bs::String head;

        bs::UINT32 bodyTextureNr;
        bs::UINT32 bodyTextureColor;

        bs::UINT32 teethTextureNr;
        bs::UINT32 teethTextureColor;

        bs::UINT32 armorInstance;
      };

      // virtual void MDL_SetVisualBody(bs::HSceneObject characterSO, const VisualBody& visual) = 0;

      struct RoutineTime
      {
        bs::INT32 hourStart;
        bs::INT32 minuteStart;
        bs::INT32 hourStop;
        bs::INT32 minuteStop;
      };

      // virtual void MDL_ApplyOverlayMds(bs::HSceneObject characterSO, const bs::String& overlay)  =
      // 0; virtual void MDL_RemoveOverlayMds(bs::HSceneObject characterSO, const bs::String&
      // overlay) = 0;

      // virtual void TA_Min(bs::HSceneObject characterSO, const RoutineTime& times) = 0;

      // virtual void EquipItem(bs::HSceneObject characterSO, const bs::String& instance)           =
      // 0; virtual void CreateInventoryItem(bs::HSceneObject characterSO, const bs::String&
      // instance) = 0; virtual void CreateInventoryItems(bs::HSceneObject characterSO, const
      // bs::String& instance,
      //                                   bs::INT32 num)                                           =
      //                                   0;

      // virtual bs::INT32 NPC_GetDistToNPC(bs::HSceneObject from, bs::HSceneObject to)           =
      // 0; virtual bs::INT32 NPC_GetDistToWaypoint(bs::HSceneObject characterSO,
      //                                         const bs::String& waypoint)                      =
      //                                         0;
      // virtual bs::INT32 NPC_GetDistToItem(bs::HSceneObject characterSO, bs::HSceneObject item) =
      // 0; virtual bs::INT32 NPC_GetDistToPlayer(bs::HSceneObject npcSO) = 0;

      // virtual bs::INT32 NPC_GetTrueGuild(bs::HSceneObject characterSO)                  = 0;
      // virtual bs::INT32 NPC_SetTrueGuild(bs::HSceneObject characterSO, bs::INT32 guild) = 0;

      // virtual void NPC_SetToFightMode(bs::HSceneObject characterSO, const bs::String& weapon) = 0;
      // virtual void NPC_SetToFistMode(bs::HSceneObject characterSO) = 0;

      // virtual bool NPC_IsPlayer(bs::HSceneObject characterSO)                                  =
      // 0; virtual bool NPC_IsDead(bs::HSceneObject characterSO) = 0; virtual bool
      // NPC_IsOnFreepoint(bs::HSceneObject characterSO, const bs::String& waypoint) = 0;

      // virtual bool NPC_CanSeeNPC(bs::HSceneObject characterSO,
      //                            bs::HSceneObject targetCharacterSO) = 0;

      // virtual bool NPC_CanSeeNpcFreeLOS(bs::HSceneObject characterSO,
      //                                   bs::HSceneObject targetCharacterSO) = 0;

      // virtual bool NPC_CanSeeItem(bs::HSceneObject characterSO, bs::HSceneObject itemSO) = 0;

      // virtual void NPC_ClearAiQueue(bs::HSceneObject characterSO)     = 0;
      // virtual void NPC_ExchangeRoutine(bs::HSceneObject characterSO,
      //                                  const bs::String& routineName) = 0;

      // virtual bs::String NPC_GetNearestWaypoint(bs::HSceneObject characterSO) = 0;
      // virtual bs::String NPC_GetNextWaypoint(bs::HSceneObject characterSO)    = 0;

      // virtual void NPC_SetRefuseTalk(bs::HSceneObject characterSO, bs::INT32 durationSeconds) = 0;
      // virtual void NPC_RefuseTalk(bs::HSceneObject characterSO)                               = 0;

      // virtual bool NPC_IsInState(bs::HSceneObject characterSO, const bs::INT32 state) = 0;

      // virtual bool NPC_HasEquippedMelleWeapon(bs::HSceneObject characterSO) = 0;

      // virtual bool NPC_KnowsInfo(bs::HSceneObject characterSO, const bs::String& instance) = 0;

      // virtual void NPC_ClearInventory(bs::HSceneObject characterSO)                      = 0;
      // virtual bool NPC_HasItem(bs::HSceneObject characterSO, const bs::String& instance) = 0;
      // virtual void NPC_GiveItem(bs::HSceneObject characterToSO, const bs::String& instance,
      //                           bs::HSceneObject characterFromSO)                        = 0;
      // virtual void NPC_RemoveInventoryItems(bs::HSceneObject characterSO, const bs::String&
      // instance,
      //                                       bs::INT32 amount)                            = 0;
      // virtual bool NPC_CheckInfo(bs::HSceneObject characterSO, bool important)           = 0;

      // virtual bs::INT32 NPC_GetStateTime(bs::HSceneObject characterSO)               = 0;
      // virtual void NPC_SetStateTime(bs::HSceneObject characterSO, bs::INT32 seconds) = 0;
      // virtual void NPC_ChangeAttribute(bs::HSceneObject characterSO, bs::INT32 attribute,
      //                                  BS::INT32 value)                              = 0;

      // virtual void AI_Teleport(bs::HSceneObject characterSO, const bs::String& waypoint)        =
      // 0; virtual void AI_TurnToNPC(bs::HSceneObject characterSO, bs::HSceneObject targetSO) = 0;
      // virtual void AI_StandUp(bs::HSceneObject characterSO)                                     =
      // 0; virtual void AI_StandUpQuick(bs::HSceneObject characterSO) = 0; virtual void
      // AI_GotoWaypoint(bs::HSceneObject characterSO, const bs::String& waypoint)    = 0; virtual
      // void AI_GotoFreePoint(bs::HSceneObject characterSO, const bs::String& freepoint)  = 0;
      // virtual void AI_GotoNextFreePoint(bs::HSceneObject characterSO,
      //                                   const bs::String& freepoint)                            =
      //                                   0;
      // virtual void AI_GotoNpc(bs::HSceneObject characterSO, bs::HSceneObject targetCharacterSO) =
      // 0; virtual void AI_StartState(bs::HSceneObject characterSO, const bs::String& stateFunction,
      //                            bs::INT32 stateBehavior, const bs::String& waypoint)           =
      //                            0;
      // virtual void AI_Wait(bs::HSceneObject characterSO, float seconds)                         =
      // 0; virtual void AI_PlayAni(bs::HSceneObject characterSO, const bs::String& animation) = 0;
      // virtual void AI_SetWalkMode(bs::HSceneObject characterSO, bs::UINT32 walkMode)            =
      // 0; virtual void AI_StopProcessingInfos(bs::HSceneObject characterSO) = 0; virtual void
      // AI_Output(bs::HSceneObject characterSO, bs::HSceneObject characterSO,
      //                        const bs::String& svmName)                                         =
      //                        0;
      // virtual void AI_ProcessInfos(bs::HSceneObject characterSO)                                =
      // 0;

      // virtual bool Infomanager_HasFinished()                         = 0;
      // virtual void Info_AddChoice(const bs::String& infoInstance, const bs::String& text,
      //                             const bs::String& function)        = 0;
      // virtual void Info_ClearChoices(const bs::String& infoInstance) = 0;

      struct PrintScreenInfo
      {
        bs::INT32 positionX;
        bs::INT32 positionY;

        bs::INT32 dialogNr;  // No idea what that is

        bs::String font;

        bs::INT32 durationSeconds;
      };

      // virtual void PrintScreen(const bs::String& message, const PrintScreenInfo& info) = 0;
      // virtual void PrintDebugInstCh(bs::INT32 channel, const bs::String& text)         = 0;

      // virtual SymbolIndex HLP_GetInstanceID(SymbolIndex index)          = 0;
      // virtual ScriptObjectHandle HLP_GetNpc(const bs::String& instance) = 0;
      // virtual bool HLP_IsValidNpc(bs::HSceneObject characterSO)         = 0;

      // virtual bs::INT32 WLD_DetectNpc(bs::HSceneObject characterSO, const bs::String& instance,
      //                                 bs::INT32 aiState, bs::INT32 guild)                       =
      //                                 0;
      // virtual bool WLD_IsTime(bs::INT32 hour1, bs::INT32 min1, bs::INT32 hour2, bs::INT32 min2) =
      // 0; virtual void WLD_IsFreepointAvailable(bs::HSceneObject characterSO,
      //                                       const bs::String& freepoint)                        =
      //                                       0;
      // virtual void WLD_IsNextFreepointAvailable(bs::HSceneObject characterSO,
      //                                           const bs::String& freepoint)                    =
      //                                           0;
      // virtual void WLD_InsertNpc(const bs::String& instance, const bs::String& spawnpoint)      =
      // 0; virtual void WLD_InsertItem(const bs::String& instance, const bs::String& spawnpoint) =
      // 0; virtual bs::INT32 WLD_GetDay() = 0; virtual bs::INT32 WLD_GetGuildAttitude(bs::INT32
      // aggressorGuild, bs::INT32 victimGuild)   = 0;

      // virtual void SND_Play(const bs::String& sound) = 0;

      // virtual void LOG_CreateTopic(const bs::String& topic, const bs::String& section)    = 0;
      // virtual void LOG_SetTopticStatus(const bs::String& topic, const bs::String& status) = 0;
      // virtual void LOG_AddEntry(const bs::String& topic, const bs::String& entry)         = 0;
      // virtual void IntroduceChapter(const bs::String& title, const bs::String& subtitle,
      //                               const bs::String& texture, const bs::String& sound,
      //                               bs::INT32 waitTimeSeconds) = 0;

      // virtual bs::INT32 MOB_HasITems(bs::HScriptCode mobSO, const bs::String& instance) = 0;
    };

    /**
     * DaedalusVM interface to the rest of the engine.
     */
    class DaedalusVMWithExternals : public DaedalusVM,
                                    public IDaedalusScriptInterface,
                                    public IDaedalusScriptExternals
    {
    public:
      DaedalusVMWithExternals(const Daedalus::DATFile& datFile);

      ScriptObjectHandle instanciateClass(const bs::String& className,
                                          const bs::String& instanceName,
                                          bs::HSceneObject mappedSceneObject) override;

      void initializeWorld(const bs::String& worldName) override;
      void setHero(ScriptObjectHandle hero) override;

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

      void script_PrintPlus(const bs::String& text) override;

      void external_Print();
      void external_HLP_Random();
      void external_HLP_GetNpc();
      void external_IntToString();
      void external_ConcatStrings();
      void external_WLD_InsertItem();
      void external_WLD_InsertNpc();
      void external_NPC_IsPlayer();
      void external_NPC_SetTalentSkill();
      void external_NPC_EquipItem();
      void external_NPC_CreateInventoryItems();
      void external_NPC_CreateInventoryItem();
      void external_MDL_SetVisual();
      void external_MDL_SetVisualBody();

      void registerAllExternals() override;
    };
  }  // namespace Scripting
}  // namespace REGoth
