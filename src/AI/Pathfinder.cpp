#include "Pathfinder.hpp"
#include <Math/BsRay.h>
#include <Math/BsVector2.h>
#include <Physics/BsPhysics.h>
#include <RTTI/RTTI_Pathfinder.hpp>
#include <Scene/BsSceneManager.h>
#include <components/GameWorld.hpp>
#include <components/Waynet.hpp>
#include <components/Waypoint.hpp>

static const float MAX_SIDE_DIFFERENCE_TO_REACH_POSITION     = 0.5f;  // Meters
static const float MAX_HEIGHT_DIFFERENCE_TO_REACH_POSITION   = 2.0f;  // Meters
static const float MAX_TARGET_ENTITY_MOVEMENT_BEFORE_REROUTE = 5.0f;  // Meters
static const float MAX_POINT_DISTANCE_FOR_CLEANUP            = 5.0f;  // Meters

namespace REGoth
{
  namespace AI
  {
    Pathfinder::Pathfinder(HWaynet waynet)
        : mWaynet(waynet)
    {
    }

    Pathfinder::~Pathfinder()
    {
    }

    Pathfinder::MovementReport Pathfinder::checkMoveToLocation(const bs::Vector3& from,
                                                               const bs::Vector3& to) const
    {
      MovementReport report;
      bs::Vector3 fromGround = from - bs::Vector3(0, mUserConfiguration.height, 0);
      bs::Vector3 toGround   = to - bs::Vector3(0, mUserConfiguration.height, 0);

      report.lowerThanStepHeight  = (to.y - from.y) < -mUserConfiguration.stepHeight;
      report.higherThanStepHeight = (to.y - from.y) > mUserConfiguration.stepHeight;

      report.ceilingTooLow = findCeilingHeightAtPosition(toGround) > mUserConfiguration.height;

      // TODO: Reimplement using bsf
      // Physics::RayTestResult hit = m_World.getPhysicsSystem().raytrace(from, to);
      // report.hardCollision       = hit.hasHit;

      // size_t destGroundTriangle = getGroundTriangleIndexAt(to);

      // if (destGroundTriangle != (size_t)-1)
      // {
      //   bs::Vector3 vertices[3];
      //   uint8_t matGroup;
      //   m_World.getWorldMesh().getTriangle(destGroundTriangle, vertices, matGroup);

      //   bs::Vector3 normal =
      //       (vertices[0] - vertices[1]).cross(vertices[0] - vertices[2]).normalize();

      //   float slope = calculateSlopeFromNormal(normal);

      //   bool goingUp = (to - from).dot(normal) < 0.0f;

      //   if (fabs(slope) > mUserConfiguration.maxSlopeAngle)
      //   {
      //     if (goingUp) report.tooSteepUp = true;
      //     if (!goingUp) report.tooSteepDown = true;
      //   }
      // }
      // else
      // {
      //   report.lowerThanStepHeight = true;
      // }

      return report;
    }

    float Pathfinder::findCeilingHeightAtPosition(const bs::Vector3& floorposition) const
    {
      return std::numeric_limits<float>::max();

      bs::PhysicsQueryHit hit;

      const bs::Vector3 up = bs::Vector3::UNIT_Y;
      if (!physicsScene().rayCast(floorposition, up, hit))
      {
        return std::numeric_limits<float>::max();
      }

      return hit.distance;
    }

    float Pathfinder::calculateSlopeFromNormal(const bs::Vector3& normal) const
    {
      return acos(normal.y);
    }

    HWaypoint Pathfinder::findNextVisibleWaypoint(const bs::Vector3& from) const
    {
      // TODO: Check for obstructions
      return mWaynet->findClosestWaypointTo(from);
    }

    bool Pathfinder::hasActiveRouteBeenCompleted(const bs::Vector3& positionNow) const
    {
      if (!mActiveRoute.targetEntity)

        // FIXME: This goes wrong if an npc ever gets stuck or the heights don't match
        return mActiveRoute.positionsToGo.empty();

      return hasTargetEntityBeenReached(positionNow);
    }

    bool Pathfinder::isTargetReachedByPosition(const bs::Vector3& position,
                                               const bs::Vector3& target)
    {
      float diffHeight = fabs(position.y - target.y);
      float diffSide   = bs::Vector2(position.x - target.x, position.z - target.z).length();

      return diffSide < MAX_SIDE_DIFFERENCE_TO_REACH_POSITION &&
             diffHeight < MAX_HEIGHT_DIFFERENCE_TO_REACH_POSITION;
    }

    Pathfinder::Instruction Pathfinder::updateToNextInstructionToTarget(
        const bs::Vector3& positionNow)
    {
      Instruction inst;

      if (hasNextRouteTargetBeenReached(positionNow))
      {
        if (!mActiveRoute.positionsToGo.empty())
        {
          mActiveRoute.positionsToGo.pop_front();
        }
      }

      if (hasActiveRouteBeenCompleted(positionNow))
      {
        inst.targetPosition = positionNow;
        return inst;
      }

      if (shouldReRoute(positionNow))
      {
        if (isTargetAnEntity())
        {
          startNewRouteTo(positionNow, mActiveRoute.targetEntity);
        }
        else
        {
          assert(!mActiveRoute.positionsToGo.empty());
          startNewRouteTo(positionNow, mActiveRoute.positionsToGo.back());
        }
      }

      inst.targetPosition = getCurrentTargetPosition(positionNow);

      return inst;
    }

    bool Pathfinder::hasNextRouteTargetBeenReached(const bs::Vector3& positionNow) const
    {
      if (mActiveRoute.positionsToGo.empty())
      {
        return isTargetAnEntity() && hasTargetEntityBeenReached(positionNow);
      }

      return isTargetReachedByPosition(positionNow, mActiveRoute.positionsToGo.front());
    }

    bool Pathfinder::hasTargetEntityBeenReached(const bs::Vector3& positionNow) const
    {
      if (!isTargetAnEntity()) return false;

      bs::Vector3 targetEntityPosition = getTargetEntityPosition();

      return isTargetReachedByPosition(positionNow, targetEntityPosition);
    }

    bs::Vector3 Pathfinder::getTargetEntityPosition() const
    {
      if (!isTargetAnEntity()) return bs::Vector3(0, 0, 0);

      return mActiveRoute.targetEntity->getTransform().pos();
    }

    bool Pathfinder::isTargetAnEntity() const
    {
      return !!mActiveRoute.targetEntity;
    }

    bs::Vector3 Pathfinder::getCurrentTargetPosition(const bs::Vector3& positionNow) const
    {
      bs::Vector3 targetEntityPosition = getTargetEntityPosition();
      if (isTargetAnEntity() && canDirectlyMovetoLocation(positionNow, targetEntityPosition))
      {
        return targetEntityPosition;
      }

      if (mActiveRoute.positionsToGo.empty())
      {
        return targetEntityPosition;
      }
      else
      {
        return mActiveRoute.positionsToGo.front();
      }
    }

    void Pathfinder::startNewRouteTo(const bs::Vector3& positionNow, bs::HSceneObject entity)
    {
      // Must be set for getTargetEntityPosition to work
      mActiveRoute.targetEntity = entity;

      startNewRouteTo(positionNow, getTargetEntityPosition());

      // startNewRouteTo appends the position to go to if it's off the waynet, we can't have that
      // when the target could be moving
      if (!mActiveRoute.positionsToGo.empty())
      {
        mActiveRoute.positionsToGo.pop_back();
      }

      mActiveRoute.targetEntity = entity;

      // Save start position to see when the entity moved too far
      mActiveRoute.targetEntityPositionOnStart = getTargetEntityPosition();
    }

    void Pathfinder::startNewRouteTo(const bs::Vector3& positionNow, const bs::Vector3& position)
    {
      mActiveRoute.positionsToGo.clear();
      mActiveRoute.lastKnownPosition   = positionNow;
      mActiveRoute.targetEntity        = {};
      mActiveRoute.isTargetUnreachable = false;

      if (isTargetReachedByPosition(positionNow, position)) return;

      if (canDirectlyMovetoLocation(positionNow, position))
      {
        mActiveRoute.positionsToGo.push_back(position);
        return;
      }

      HWaypoint nearestWpToTarget = mWaynet->findClosestWaypointTo(position);
      HWaypoint nearestWpToStart  = mWaynet->findClosestWaypointTo(positionNow);

      bs::Vector<HWaypoint> path = mWaynet->findWay(nearestWpToStart, nearestWpToTarget);

      if (path.empty())
      {
        // We already checked whether we can directly move here. So since it's not possible
        // via the waynet, just exit here.
        mActiveRoute.isTargetUnreachable = true;

        bs::gDebug().logDebug(bs::StringUtil::format("[Pathfinder] No path from {0} to {1}",
                                                     nearestWpToStart->getName(),
                                                     nearestWpToTarget->getName()));
        return;
      }

      for (auto wp : path)
      {
        const bs::Vector3& wpPosition = wp->SO()->getTransform().pos();

        mActiveRoute.positionsToGo.push_back(wpPosition);
      }

      bool isDestinationOffWaynet = false;

      if (mActiveRoute.positionsToGo.empty())
      {
        isDestinationOffWaynet = true;
      }

      if (!isTargetReachedByPosition(mActiveRoute.positionsToGo.back(), position))
      {
        isDestinationOffWaynet = true;
      }

      // If the last position is off the waynet, add it as explicit position
      if (isDestinationOffWaynet)
      {
        mActiveRoute.positionsToGo.push_back(position);
      }

      cleanupRoute();
    }

    bool Pathfinder::canDirectlyMovetoLocation(const bs::Vector3& from, const bs::Vector3& to) const
    {
      if (isTargetReachedByPosition(from, to)) return true;

      bs::PhysicsQueryHit hit;

      // FIXME: This breaks when the creature should go down a slope but is
      // standing on the top of it right now

      bs::Vector3 dir = to - from;
      float distance  = dir.length();

      dir /= distance;

      if (!physicsScene().rayCast(from, dir, hit))
      {
        return true;
      }

      // It's okay if the hit is furhter back than our target point
      return hit.distance > distance;
    }

    void Pathfinder::debugDrawRoute(const bs::Vector3& positionNow)
    {
      // TODO: Implement using bsf
    }

    bool Pathfinder::hasTargetEntityMovedTooFar() const
    {
      if (!isTargetAnEntity()) return false;

      // Note: Squared for performance
      float targetMoveDistanceSq =
          (mActiveRoute.targetEntityPositionOnStart - getTargetEntityPosition()).squaredLength();
      float maxTargetMoveDistanceSq =
          MAX_TARGET_ENTITY_MOVEMENT_BEFORE_REROUTE * MAX_TARGET_ENTITY_MOVEMENT_BEFORE_REROUTE;

      return targetMoveDistanceSq > maxTargetMoveDistanceSq;
    }

    void Pathfinder::cleanupRoute()
    {
      // Outline: For each point, trace to the next points until we find one we can't directly go to.
      //          Remove all points between the one before that and the one we're currently looking
      //          at.
      //
      //          There is also a maximum distance these point can be apart from each other, so NPCs
      //          would still respect paths on the worldmesh.

      if (mActiveRoute.positionsToGo.size() < 3) return;

      bool removed;

      do
      {
        removed = false;
        for (auto it = mActiveRoute.positionsToGo.begin(); it != mActiveRoute.positionsToGo.end();
             it++)
        {
          if (canRoutePositionBeRemoved(it))
          {
            it      = mActiveRoute.positionsToGo.erase(it);
            removed = true;
          }
        }
      } while (removed);
    }

    bool Pathfinder::canRoutePositionBeRemoved(std::list<bs::Vector3>::iterator it)
    {
      if (it == mActiveRoute.positionsToGo.begin()) return false;

      if (it == mActiveRoute.positionsToGo.end()) return false;

      if (it == std::prev(mActiveRoute.positionsToGo.end())) return false;

      auto prev = std::prev(it);
      auto next = std::next(it);

      float distToPrevSq    = ((*it) - (*prev)).squaredLength();
      float maxDistToPrevSq = MAX_POINT_DISTANCE_FOR_CLEANUP * MAX_POINT_DISTANCE_FOR_CLEANUP;

      // Only remove points which aren't too far appart
      if (distToPrevSq > maxDistToPrevSq) return false;

      const bool samePosition =
          isTargetReachedByPosition(*prev, *it) || isTargetReachedByPosition(*next, *it);

      if (samePosition) return true;

      const bool detour = isTargetReachedByPosition(*prev, *next);

      if (detour) return true;

      bool canMoveDirectlytoNext = canDirectlyMovetoLocation(*prev, *next);

      return canMoveDirectlytoNext;
    }

    bool Pathfinder::shouldReRoute(const bs::Vector3& positionNow) const
    {
      // FIXME: canDirectlyMovetoLocation fails if the npc should move up/down a (walkable) hill like
      // that:
      //
      //        ________x
      //       /
      //  _x__/
      //
      // Notice how the hill is blocking the view to the two waypoints, but the slope could still be
      // walkable!

      if (mActiveRoute.isTargetUnreachable)
      {
        // We have already given up, makes no sense to try again
        return false;
      }

      if (!isTargetAnEntity())
      {
        if (mActiveRoute.positionsToGo.empty())
        {
          return false;
        }
      }

      // Makes no sense without moving target from here on
      if (!isTargetAnEntity())
      {
        return false;
      }

      if (mActiveRoute.positionsToGo.empty())
      {
        if (!canDirectlyMovetoLocation(positionNow, getTargetEntityPosition()))
        {
          return true;
        }
      }

      if (hasTargetEntityMovedTooFar())
      {
        return true;
      }

      return false;
    }

    bs::PhysicsScene& Pathfinder::physicsScene() const
    {
      auto p = bs::gSceneManager().getMainScene()->getPhysicsScene();

      if (!p)
      {
        REGOTH_THROW(InvalidStateException, "Needs a physics scene!");
      }

      return *p;
    }

    REGOTH_DEFINE_RTTI(Pathfinder)
  }  // namespace AI
}  // namespace REGoth
