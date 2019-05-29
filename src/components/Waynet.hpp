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

  class Freepoint;
  using HFreepoint = bs::GameObjectHandle<Freepoint>;

  class AnchoredTextLabels;
  using HAnchoredTextLabels = bs::GameObjectHandle<AnchoredTextLabels>;

  /**
   * Waynet of a world.
   *
   * The Waynet is used by the characters for navigation and some
   * other scripting related things like placing an item at a
   * certain location.
   *
   * It consists out of Nodes called Waypoints, which have bi-directional
   * connections to other nodes. To naviage somewhere, the shortest path between
   * two nodes is taken.
   *
   * How exactly Gothic uses the waynet is still unknown. Tests with a simple
   * pathfinding algorithm have shown that there must be more to it. For
   * example:
   *
   *  In REGoth, before bs:f, the character showing the way from the old camp to
   *  the swamp will go though the forest, while he goes around the forest in
   *  the original.
   *
   * Maybe the characters are advised to go through the Outside-world, if
   * possible?
   *
   *
   * Freepoints and Waypoints
   * ========================
   *
   * The original engine uses Waypoints for most of the navigation. However,
   * the term *Freepoint* sometimes pops up in the same context. A Freepoint
   * is, even if the names are similar, fundamentally different from a
   * Waypoint. While a Waypoint is a specialized part of the Waynet, used for
   * navigation, a Freepoint is just a simple spot in the world.
   *
   * Those spots describe that a certain location could be used by a Character to
   * do something, without needing an object to interact with. For example,
   * there could be multiple Freepoints called `SIT` located around a fireplace.
   * Characters which are told to sit down by their daily routine will search
   * for a non-occupied freepoint with the name `SIT` and if they find one,
   * walk to it, mark the Freepoint as occupied and play the sitting-animation.
   *
   * Another use-case for Freepoints is for Monsters to know where to walk when
   * they are supposed to roam around their home-location. You will find multiple
   * Freepoints called `ROAM` scattered near monsters which they walk to in a
   * random order every so often.
   *
   *
   * Scene Object structure
   * ======================
   *
   * The object this component is attached to will be the parent of all
   * Waypoints this waynet has. So to search for a waypoint, you could simply
   * search the children of the Waynet-SO. Since this is a rather slow
   * operation, see findWaypoint() for alternatives.
   *
   */
  class Waynet : public bs::Component
  {
  public:
    Waynet(const bs::HSceneObject& parent);

    /**
     * Finds a waypoint by name.
     *
     * Note that this will *only* look for actual waypoints. In Gothic, while the name
     * Waypoint means exactly what one expects, when a script-function asks for a waypoint
     * it could also mean a freepoint. So if you want to be on the safe-side, then use
     * GameWorld::findObjectByName(), which is also faster.
     *
     * @param  name  Name of the Waypoint to look for.
     *
     * @return Handle to the waypoint. Invalid if not found.
     */
    HWaypoint findWaypoint(const bs::String& name);

    /**
     * Searches the closest waypoint to the given position.
     *
     * Does not check whether the waypoint is obstructed by anything and
     * ignores all waypoint connections.
     *
     * @param  position  Position to search around.
     *
     * @return Closest Waypoint to the given position. Should only be empty
     *         if no waypoint exists at all.
     */
    HWaypoint findClosestWaypointTo(const bs::Vector3& position);

    /**
     * Searches the *second* closest Waypoint to the given position,
     * not the closest one, but the one which is closest after that one.
     *
     * Does not check whether the waypoint is obstructed by anything and
     * ignores all waypoint connections.
     *
     * @param  position  Position to search around.
     *
     * @return Closest Waypoint to the given position. Should only be empty
     *         if no waypoint exists at all.
     */
    HWaypoint findSecondClosestWaypointTo(const bs::Vector3& position);

    /**
     * Searches the closest Freepoint to the given position.
     *
     * Does not check whether the Freepoint is obstructed by anything and
     * ignores all waypoint connections.
     *
     * @param  position  Position to search around.
     *
     * @return Closest Freepoint to the given position. Should only be empty
     *         if no Freepoint exists at all.
     */
    HFreepoint findClosestFreepointTo(const bs::String& name, const bs::Vector3& position);

    /**
     * Searches the *second* closest Freepoint to the given position,
     * not the closest one, but the one which is closest after that one.
     *
     * Does not check whether the Freepoint is obstructed by anything and
     * ignores all waypoint connections.
     *
     * @param  position  Position to search around.
     *
     * @return Closest Freepoint to the given position. Should only be empty
     *         if no Freepoint exists at all.
     */
    HFreepoint findSecondClosestFreepointTo(const bs::String& name, const bs::Vector3& position);

    /**
     * Finds a way between two waypoints in the given waypoint instance
     *
     * @return List of all waypoints that need to be visited. Will be empty if none was found.
     */
    bs::Vector<HWaypoint> findWay(HWaypoint from, HWaypoint to);

    /**
     * Registers the given waypoint in the waynet.
     */
    void addFreepoint(HFreepoint freepoint);

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
     * @return List of all freepoints.
     */
    const bs::Vector<HFreepoint>& allFreepoints()
    {
      return mFreepoints;
    }

    /**
     * Draws the waynet as lines.
     */
    void debugDraw(const REGoth::HAnchoredTextLabels& textLabels);

  private:

    /**
     * Fills mWaypointPositions with the positions from all registered waypoints.
     *
     * Will drop anything already in the vector and thus can be called multiple times.
     */
    void populateWaypointPositionCache();
    void populateFreepointPositionCache();

    /**
     * @return Whether mWaypointPositions has been filled with data.
     *
     * @note To fill it, use populateWaypointPositionCache().
     */
    bool hasCachedWaypointPositions() const;
    bool hasCachedFreepointPositions() const;

    bs::Vector<HWaypoint> mWaypoints;
    bs::Vector<HFreepoint> mFreepoints;

    /**
     * Cached positions for faster access during searches.
     * Waypoints are supposed to be static, so it's okay to cache these.
     */
    bs::Vector<bs::Vector3> mWaypointPositions;
    bs::Vector<bs::Vector3> mFreepointPositions;

  public:
    REGOTH_DECLARE_RTTI(Waynet)

  protected:
    Waynet() = default; // For RTTI
  };
}  // namespace REGoth
