#pragma once
#include "ScriptBackedBy.hpp"
#include <BsPrerequisites.h>

namespace REGoth
{
  /**
   * Character AI. Implements most of the `*` externals.
   * Needs to be attached to a scene-object which also has s `Character`-Component.
   */
  class CharacterAI : public bs::Component
  {
  public:
    CharacterAI(const bs::HSceneObject& parent);

    void teleport(const bs::String& waypoint);
    void turnToNPC(bs::HSceneObject targetSO);
    void standUp();
    void standUpQuick();
    void gotoWaypoint(const bs::String& waypoint);
    void gotoFreePoint(const bs::String& freepoint);
    void gotoNextFreePoint(const bs::String& freepoint);
    void gotoNpc(bs::HSceneObject targetCharacterSO);
    void startState(const bs::String& stateFunction, bs::INT32 stateBehavior,
                    const bs::String& waypoint);
    void wait(float seconds);
    void playAni(const bs::String& animation);
    void setWalkMode(bs::UINT32 walkMode);
    void stopProcessingInfos();
    void output(bs::HSceneObject characterSO, const bs::String& svmName);
    void processInfos();
  };
}  // namespace REGoth
