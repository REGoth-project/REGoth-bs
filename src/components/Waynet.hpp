#pragma once
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>
#include <RTTI/RTTIUtil.hpp>

namespace REGoth
{
  class Waynet;
  using HWaynet = bs::GameObjectHandle<Waynet>;

  class Waypoint;
  using HWaypoint = bs::GameObjectHandle<Waypoint>;

  class AnchoredTextLabels;
  using HAnchoredTextLabels = bs::GameObjectHandle<AnchoredTextLabels>;

  /**
   * Waynet of a world.
   *
   * The Waynet is used by the characters for navigation and some
   * other scripting related things like placing an item at a
   * certain location.
   *
   * It consists out of Nodes, which have bi-directional connections to other nodes.
   * To naviage somewhere, the shortest path between two nodes is taken.
   *
   * How exactly Gothic uses the waynet is still unknown. Tests with a simple
   * pathfinding algorithm have shown that there must be more to it. For example:
   *
   *  In REGoth, before bs:f, the character showing the way from the old camp to the
   *  swamp will go though the forest, while he goes around the forest in the original.
   *
   * Maybe the characters are advised to go through the Outside-world, if possible?
   *
   *
   * # Scene Object structure
   *
   * The object this component is attached to will be the parent of all Waypoints
   * this waynet has. So to search for a waypoint, simply search the children of
   * the Waynet-SO.
   */
  class Waynet : public bs::Component
  {
  public:
    Waynet(const bs::HSceneObject& parent);

    /**
     * Finds a waypoint by name.
     *
     * @param  name  Name of the Waypoint to look for.
     *
     * @return Handle to the waypoint. Invalid if not found.
     */
    HWaypoint findWaypoint(const bs::String& name);

    /**
     * Registers the given waypoint in the waynet.
     */
    void addWaypoint(HWaypoint waypoint);

    /**
     * @return List of all waypoints.
     */
    const bs::Vector<HWaypoint>& allWaypoints()
    {
      return mWaypoints;
    }

    /**
     * Draws the waynet as lines.
     */
    void debugDraw(const REGoth::HAnchoredTextLabels& textLabels);

  private:
    bs::Vector<HWaypoint> mWaypoints;

  public:
    REGOTH_DECLARE_RTTI(Waynet)

  protected:
    Waynet() = default; // For RTTI
  };
}  // namespace REGoth
