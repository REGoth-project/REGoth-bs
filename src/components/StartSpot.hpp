#pragma once

#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>

namespace REGoth
{
  /**
   * Component marking the scene object it is attached to as starting spot
   * for the player. That would be at the exchange place in Gothic I, for example.
   */
  class StartSpot : public bs::Component
  {
  public:
    StartSpot(const bs::HSceneObject& parent);
  };
}
