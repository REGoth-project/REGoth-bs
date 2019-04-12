#include "Waynet.hpp"
#include <Debug/BsDebugDraw.h>
#include <Scene/BsSceneObject.h>
#include <components/Waypoint.hpp>

namespace REGoth
{
  Waynet::Waynet(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
  }

  HWaypoint Waynet::findWaypoint(const bs::String& name)
  {
    bs::HSceneObject waypointSO = SO()->findChild(name);

    if (waypointSO) return {};

    return waypointSO->getComponent<Waypoint>();
  }

  void Waynet::addWaypoint(HWaypoint waypoint)
  {
    mWaypoints.push_back(waypoint);
  }

  void Waynet::debugDraw()
  {
    bs::DebugDraw::instance().setColor(bs::Color::Red);

    for (HWaypoint from : allWaypoints())
    {
      for (HWaypoint to : from->allPaths())
      {
        bs::DebugDraw::instance().drawLine(from->SO()->getTransform().pos(),
                                           to->SO()->getTransform().pos());
      }
    }
  }

}  // namespace REGoth
