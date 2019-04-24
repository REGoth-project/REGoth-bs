#include "Waynet.hpp"
#include <Debug/BsDebugDraw.h>
#include <Scene/BsSceneObject.h>
#include <components/AnchoredTextLabels.h>
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

    if (!waypointSO) return {};

    return waypointSO->getComponent<Waypoint>();
  }

  void Waynet::addWaypoint(HWaypoint waypoint)
  {
    mWaypoints.push_back(waypoint);
  }

  void Waynet::debugDraw(const REGoth::HAnchoredTextLabels& textLabels)
  {
    bs::DebugDraw::instance().setColor(bs::Color::Red);

    for (HWaypoint from : allWaypoints())
    {
      const bs::Transform& fromTransform = from->SO()->getTransform();

      textLabels->addLabel(fromTransform.pos() + fromTransform.getUp() * 0.5f,
                           bs::HString(from->SO()->getName()));
      for (HWaypoint to : from->allPaths())
      {
        bs::DebugDraw::instance().drawLine(fromTransform.pos(), to->SO()->getTransform().pos());
      }
    }
  }

}  // namespace REGoth
