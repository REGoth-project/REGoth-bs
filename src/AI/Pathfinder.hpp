#pragma once
#include <BsCorePrerequisites.h>
#include <Math/BsVector3.h>

namespace bs
{
  class PhysicsScene;
}
namespace REGoth
{
  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  class Waypoint;
  using HWaypoint = bs::GameObjectHandle<Waypoint>;

  namespace AI
  {
    /**
     * Wrapper around the waynet. Can find paths to certain locations and
     * give instructions onto how to get there from a given point.
     */
    class Pathfinder
    {
    public:
      /**
       * The next point on the route to go to in order to reach the final destination.
       */
      struct Instruction
      {
        bs::Vector3 targetPosition;
      };

      struct UserConfiguration
      {
        float height;
        float radius;
        float stepHeight;
        float maxSlopeAngle;
      };

      struct Route
      {
        bs::Vector3 lastKnownPosition;

        bs::List<bs::Vector3> positionsToGo;

        // If this is valid, the Creature will move to this entity, once it has been to
        // all positions it had to go to or has a direct line of sight to it
        bs::HSceneObject targetEntity;

        // Position the target entity was at when we started the route.
        // This must be saved so we can perform a re-route if the target moves too far from the spot
        // the route goes to
        bs::Vector3 targetEntityPositionOnStart;
      };

      Pathfinder(HGameWorld world);
      virtual ~Pathfinder();

      void startNewRouteTo(const bs::Vector3& positionNow, const bs::Vector3& target);
      void startNewRouteTo(const bs::Vector3& positionNow, bs::HSceneObject entity);

      /**
       * @return The next target position on the way to the location set via startNewRouteTo.
       *         If the target has been reached, positionNow is returned, so you should always check
       *         whether the route has been completed first.
       */
      Instruction updateToNextInstructionToTarget(const bs::Vector3& positionNow);

      /**
       * @return Whether the active route has been completed.
       */
      bool hasActiveRouteBeenCompleted(const bs::Vector3& positionNow) const;

      /**
       * Information about the creature using this pathfinder
       */
      void setConfiguration(const UserConfiguration& configuration)
      {
        mUserConfiguration = configuration;
      }

      const UserConfiguration& getConfiguration() const
      {
        return mUserConfiguration;
      }

      /**
       * @return Whether the Pathfinder thinks the given targetposition has been reached
       */
      static bool isTargetReachedByPosition(const bs::Vector3& position,
                                            const bs::Vector3& target);

    private:
      struct MovementReport
      {
        bool lowerThanStepHeight;
        bool higherThanStepHeight;

        bool tooSteepDown;
        bool tooSteepUp;
        bool ceilingTooLow;
        bool hardCollision;

        // Handle::EntityHandle hitVob; // TODO: This could be useful
      };

      /**
       * Gives information about what would happen if an NPC moved to a certain location.
       * @param from Source location. Expected to be at (Groundlevel + Height) of the creature
       * @param to   Destination. Expected to be at (Groundlevel + Height) of the creature
       */
      MovementReport checkMoveToLocation(const bs::Vector3& from, const bs::Vector3& to) const;

      /**
       * Performs a series of checks on whether the creature could directly walk to the given
       * location
       */
      bool canDirectlyMovetoLocation(const bs::Vector3& from, const bs::Vector3& to) const;

      /**
       * @param  floorposition  Position on the floor. The length from that position to the
       *                        ceiling is returned.
       *
       * @return height of the ceiling at the given position
       */
      float findCeilingHeightAtPosition(const bs::Vector3& floorposition) const;

      /**
       * @return Slope angle in radians
       */
      float calculateSlopeFromNormal(const bs::Vector3& normal) const;

      /**
       * Finds the next visible waypoint from the given location.
       */
      HWaypoint findNextVisibleWaypoint(const bs::Vector3& from) const;

      /**
       * @return Whether the next position on the route has been reached
       */
      bool hasNextRouteTargetBeenReached(const bs::Vector3& positionNow) const;

      /**
       * @return Whether the entity given in the active route has been reached.
       *         False if none was specified there.
       */
      bool hasTargetEntityBeenReached(const bs::Vector3& positionNow) const;

      /**
       * @return Position of the entity to go to stored in the active route. Undefined if none was
       * specified there.
       */
      bs::Vector3 getTargetEntityPosition() const;

      /**
       * @return Whether the Target specified in the active route is an entity
       */
      bool isTargetAnEntity() const;

      /**
       * @return Position of the route to go to right now
       *
       * @Note Result is undefined when the target has been reached and there is no entity-target.
       */
      bs::Vector3 getCurrentTargetPosition(const bs::Vector3& positionNow) const;

      /**
       * Draws lines on where to go to
       */
      void debugDrawRoute(const bs::Vector3& positionNow);

      /**
       * @return Whether the target entity has moved too far from the spot it was when we started the
       * route
       */
      bool hasTargetEntityMovedTooFar() const;

      /**
       * Sometimes, the waynet isn't exactly detailed and NPCs take some weird looking detours
       * instead of going straight. This function uses raytraces to check which points on the route
       * can be erased because there are not obstacles on the way to them
       */
      void cleanupRoute();

      /**
       * @return Whether the route-position on the given iterator can be removed. See cleanupRoute().
       */
      bool canRoutePositionBeRemoved(std::list<bs::Vector3>::iterator it);

      /**
       * @return Whether the currently active route is considered not up-to-date and should be
       *         redone. This happens for example, when a target entity moved too far or something is
       *         blocking the way now.
       */
      bool shouldReRoute(const bs::Vector3& positionNow) const;

      /**
       * Access to the physics scene
       */
      bs::PhysicsScene& physicsScene() const;

      /**
       * Information about the creature using this pathfinder
       */
      UserConfiguration mUserConfiguration;

      /**
       * The currently ran route. The Creature will report it's poisition and get a new target
       * location, until it reaches the destination.
       */
      Route mActiveRoute;

      HGameWorld mWorld;
    };
  }  // namespace AI
}  // namespace Logic
