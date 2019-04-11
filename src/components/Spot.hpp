#pragma once
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>

namespace REGoth
{
  /**
   * Simple spot, like Freepoints. Doesn't do much other than being invisible
   * and having a name you can search for. This components job is to make the
   * spots searchable in the scene.
   */
  class Spot : public bs::Component
  {
  public:
    Spot(const bs::HSceneObject& parent);
  };
}
