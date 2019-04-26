#pragma once
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class Waypoint;
  using HWaypoint = bs::GameObjectHandle<Waypoint>;

  /**
   * See Waynet.
   *
   * Waypoints don't do much other than being invisible
   * and having a name you can search for and having a list of waypoints this one
   * is connected to.
   */
  class Waypoint : public bs::Component
  {
  public:
    Waypoint(const bs::HSceneObject& parent);

    /**
     * Adds a path from this waypoint to the given one.
     */
    void addPathTo(HWaypoint waypoint);

    /**
     * @return All paths from this waypoint.
     */
    const bs::Vector<HWaypoint> allPaths() const
    {
      return mPaths;
    }

  private:

    /**
     * Other waypoint this one is connected to.
     */
    bs::Vector<HWaypoint> mPaths;

  public:
    REGOTH_DECLARE_RTTI(Waypoint)

  public: // FIXME: RTTI, make protected
    Waypoint() = default;
  };
}
