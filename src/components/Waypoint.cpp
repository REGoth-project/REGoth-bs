#include "Waypoint.hpp"

namespace REGoth
{
  Waypoint::Waypoint(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
  }

  void Waypoint::addPathTo(HWaypoint waypoint)
  {
    mPaths.push_back(waypoint);
  }

}  // namespace REGoth
