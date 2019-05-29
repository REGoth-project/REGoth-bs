#include "Waynet.hpp"
#include <Debug/BsDebugDraw.h>
#include <RTTI/RTTI_Waynet.hpp>
#include <Scene/BsSceneObject.h>
#include <components/AnchoredTextLabels.h>
#include <components/Freepoint.hpp>
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
    mWaypoints.back()->mIndex = mWaypoints.size() - 1;
  }

  void Waynet::addFreepoint(HFreepoint freepoint)
  {
    mFreepoints.push_back(freepoint);
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

  Waynet::ClosestWaypoints Waynet::findClosestWaypointTo(const bs::Vector3& position)
  {
    if (!hasCachedWaypointPositions())
    {
      populateWaypointPositionCache();
    }

    // No waypoints at all?
    if (mWaypointPositions.empty())
    {
      return {};
    }

    bs::UINT32 secondNearestIndex = 0;
    float secondNearestDistance   = std::numeric_limits<float>().max();

    bs::UINT32 nearestIndex = 0;
    float nearestDistance   = std::numeric_limits<float>().max();

    for (bs::UINT32 i = 0; i < (bs::UINT32)mWaypointPositions.size(); i++)
    {
      float thisDistance = (position - mWaypointPositions[i]).squaredLength();

      if (thisDistance < nearestDistance)
      {
        nearestDistance = thisDistance;
        nearestIndex    = i;
      }

      if (thisDistance > nearestDistance && thisDistance < secondNearestDistance)
      {
        secondNearestDistance = thisDistance;
        secondNearestIndex    = i;
      }
    }

    ClosestWaypoints result;
    result.closest       = mWaypoints[nearestIndex];
    result.secondClosest = mWaypoints[secondNearestIndex];

    return result;
  }

  Waynet::ClosestFreepoints Waynet::findClosestFreepointTo(const bs::String& name,
                                                           const bs::Vector3& position)
  {
    if (!hasCachedFreepointPositions())
    {
      populateFreepointPositionCache();
    }

    // No freepoints at all?
    if (mFreepointPositions.empty())
    {
      return {};
    }

    bs::UINT32 secondNearestIndex = 0;
    float secondNearestDistance   = std::numeric_limits<float>().max();

    bs::UINT32 nearestIndex = 0;
    float nearestDistance   = std::numeric_limits<float>().max();

    for (bs::UINT32 i = 0; i < (bs::UINT32)mFreepointPositions.size(); i++)
    {
      float thisDistance = (position - mFreepointPositions[i]).squaredLength();

      if (thisDistance < nearestDistance)
      {
        nearestDistance = thisDistance;
        nearestIndex    = i;
      }

      if (thisDistance > nearestDistance && thisDistance < secondNearestDistance)
      {
        secondNearestDistance = thisDistance;
        secondNearestIndex    = i;
      }
    }

    ClosestFreepoints result;
    result.closest       = mFreepoints[nearestIndex];
    result.secondClosest = mFreepoints[secondNearestIndex];

    return result;
  }

  bs::Vector<HWaypoint> Waynet::findWay(HWaypoint from, HWaypoint to)
  {
    // FIXME: This is not a very fast implementation. Improve!
    // FIXME: The FIXME above is as old as old REGoth. And now part of REGoth-bs! Wohoo!
    //        Please please, somebody, improve this. This code is absolutely bat-shit ugly.
    //        Not even I understand it anymore. When I wrote this, only God and I were knew
    //        how it worked. Now only God knows.
    //
    //        Commets tagged with PM are from Present Me. Have fun!

    // Simple Dijkstra-Implementation. Totally non-optimized.

    // Give all other nodes a distance of infinity // PM: What other nodes?!
    std::vector<float> distances(mWaypoints.size(), FLT_MAX);
    std::vector<size_t> prev(mWaypoints.size(), static_cast<size_t>(-1));
    std::set<size_t> unvisitedSet;

    // Fill a set with 0..n, all nodes are unvisited right now
    for (size_t i = 0; i < mWaypoints.size(); i++)
    {
      unvisitedSet.insert(i);
    }

    // Init startnode with a distance of 0
    distances[from->mIndex] = 0.0f;

    // PM: Awesome coding style #1: Use names as short as possible! This might have made
    // sense as some article on the algorithm used the same name, but who knows
    // where that may be? Anyways, could mean "current node", could mean something else.
    size_t cn = from->mIndex;

    do
    {
      // PM: I don't even know, too many loops, too deeply nested.
      // Whatever, let's hope it doesn't break..
      for (auto e : mWaypoints[cn]->allPaths())
      {
        bs::UINT32 index = e->mIndex;
        if (unvisitedSet.find(index) != unvisitedSet.end())
        {
          // Check if this actually was a shorter path
          float tentativeDist =
              distances[cn] + (mWaypointPositions[cn] - mWaypointPositions[index]).squaredLength();
          if (distances[index] > tentativeDist)
          {
            distances[index] = tentativeDist;
            prev[index]      = cn;
          }
        }
      }

      unvisitedSet.erase(cn);

      if (!unvisitedSet.empty())
      {
        size_t smallest = *unvisitedSet.begin();
        for (size_t n : unvisitedSet)
        {
          if (distances[smallest] > distances[n])
          {
            smallest = n;
          }
        }

        cn = smallest;
      }

      // PM: Now that's a condition. Good look figuring that out, ouch. Look, we even have a
      // unsigned -1 in there! I love unsigned negative numbers!
    } while (unvisitedSet.find(to->mIndex) != unvisitedSet.end() && cn != static_cast<size_t>(-1) &&
             cn != to->mIndex);

    // No path found
    if (cn == static_cast<size_t>(-1)) return {};

    // Put path together
    bs::Vector<HWaypoint> path;
    cn = to->mIndex;

    while (prev[cn] != static_cast<size_t>(-1))
    {
      path.push_back(mWaypoints[cn]);
      cn = prev[cn];
    }

    // Happens on short paths // PM: What are you talking about? Back of the path not being the same
    //                        // as the starting pos? How does that happen??
    if (!path.empty() && path.back() != from)
    {
      path.push_back(from);
    };

    // PM: Seems like I fucked up the order at some point and just rolled with reversing afterwards.
    //     Nice! Efficient!
    std::reverse(path.begin(), path.end());

    return path;
  }

  void Waynet::populateWaypointPositionCache()
  {
    mWaypointPositions.clear();
    mWaypointPositions.reserve(mWaypoints.size());

    for (HWaypoint wp : allWaypoints())
    {
      mWaypointPositions.push_back(wp->SO()->getTransform().pos());
    }
  }

  void Waynet::populateFreepointPositionCache()
  {
    mFreepointPositions.clear();
    mFreepointPositions.reserve(mFreepoints.size());

    for (HFreepoint fp : allFreepoints())
    {
      mFreepointPositions.push_back(fp->SO()->getTransform().pos());
    }
  }

  bool Waynet::hasCachedWaypointPositions() const
  {
    return !mWaypointPositions.empty();
  }

  bool Waynet::hasCachedFreepointPositions() const
  {
    return !mFreepointPositions.empty();
  }

  REGOTH_DEFINE_RTTI(Waynet)

}  // namespace REGoth
