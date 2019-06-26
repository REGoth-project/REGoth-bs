#include "GameClock.hpp"
#include <Math/BsMath.h>
#include <RTTI/RTTI_GameClock.hpp>
#include <Scene/BsSceneObject.h>

namespace REGoth
{
  constexpr float SECONDS_IN_A_MINUTE = 60;
  constexpr float SECONDS_IN_AN_HOUR  = 60 * SECONDS_IN_A_MINUTE;
  constexpr float SECONDS_IN_A_DAY    = 24 * SECONDS_IN_AN_HOUR;
  // see
  // https://forum.worldofplayers.de/forum/threads/396326?p=6231841&viewfull=1#post6231841
  constexpr float CLOCK_SPEED_FACTOR = SECONDS_IN_A_DAY / 6000;

  GameClock::GameClock(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
  }

  void GameClock::fixedUpdate()
  {
    float delta = bs::gTime().getFixedFrameDelta();

    mElapsedSeconds += delta;
    mElapsedIngameSeconds += delta * CLOCK_SPEED_FACTOR;
  }

  bs::INT32 GameClock::getDay() const
  {
    return bs::Math::floorToPosInt(mElapsedIngameSeconds / SECONDS_IN_A_DAY);
  }

  bs::INT32 GameClock::getHour() const
  {
    return bs::Math::floorToPosInt(getDaySeconds() / SECONDS_IN_AN_HOUR);
  }

  bs::INT32 GameClock::getMinute() const
  {
    float elapsedSecondsOfHour = getDaySeconds() - (getHour() * SECONDS_IN_AN_HOUR);

    return bs::Math::floorToPosInt(elapsedSecondsOfHour / SECONDS_IN_A_MINUTE);
  }

  float GameClock::getDayRatio() const
  {
    return getDaySeconds() / SECONDS_IN_A_DAY;
  }

  float GameClock::getDaySeconds() const
  {
    return mElapsedIngameSeconds - (getDay() * SECONDS_IN_A_DAY);
  }

  bool GameClock::isTime(bs::INT32 hour1, bs::INT32 min1, bs::INT32 hour2, bs::INT32 min2) const
  {
    // TODO: Do we need to handle negative input parameters smaller than -24?
    hour1 = (hour1 + 24) % 24;
    min1  = (min1 + 60) % 60;
    hour2 = (hour2 + 24) % 24;
    min2  = (min2 + 60) % 60;

    float currentTimeOfDayInSeconds = getDaySeconds();
    float firstTimeOfDayInSeconds   = (hour1 * SECONDS_IN_AN_HOUR + min1 * SECONDS_IN_A_MINUTE);
    float secondTimeOfDayInSeconds  = (hour2 * SECONDS_IN_AN_HOUR + min2 * SECONDS_IN_A_MINUTE);

    if (firstTimeOfDayInSeconds != secondTimeOfDayInSeconds)
    {
      // Gothic subtracts one minute from the second time here to counter issues happening
      // with overlapping times for Daily Routines. - markusobi
      secondTimeOfDayInSeconds -= 60.0;
    }

    bool retval;
    if (hour1 == hour2 && min1 == min2)
    {
      retval = false;
    }
    else if (firstTimeOfDayInSeconds > secondTimeOfDayInSeconds)
    {
      retval = (currentTimeOfDayInSeconds >= firstTimeOfDayInSeconds) ||
               (currentTimeOfDayInSeconds <= secondTimeOfDayInSeconds);
    }
    else /* firstTimeOfDayInSeconds < secondTimeOfDayInSeconds */
    {
      retval = (currentTimeOfDayInSeconds >= firstTimeOfDayInSeconds) &&
               (currentTimeOfDayInSeconds <= secondTimeOfDayInSeconds);
    }

    return retval;
  }

  void GameClock::setTime(bs::INT32 hour, bs::INT32 min)
  {
    // TODO: Negative input parameters are supported for hour, but what about negative values that
    // result in negative remaining hours?
    //       Do we need to handle negative Minutes at all?
    bs::INT32 daysToAdvance = hour / 24;
    bs::INT32 day           = getDay() + daysToAdvance;
    hour                    = hour % 24;
    min                     = min % 60;

    setTime((bs::UINT32)day, (bs::UINT8)hour, (bs::UINT8)min);

    // TODO: According to
    //       https://forum.worldofplayers.de/forum/threads/396326?p=6231841&viewfull=1#post6231841
    //       if this shall be the Wld_setTime external, it also needs to implement these three
    //       functions here RoutineManager.SetDailyRoutinePos(RoutinesOnly)
    //       Game.SetObjectRoutineTimeChange(GameHour, GameMinute, Hour, Minute)
    //       SpawnManager.SpawnImmediately(ResetSpawnTime)
  }

  void GameClock::setDay(bs::UINT32 day)
  {
    bs::UINT8 hour = (bs::UINT8)getHour();
    bs::UINT8 min  = (bs::UINT8)getMinute();

    setTime(day, hour, min);
  }

  void GameClock::setTime(bs::UINT32 day, bs::UINT8 hour, bs::UINT8 min)
  {
    mElapsedIngameSeconds =
        day * SECONDS_IN_A_DAY + hour * SECONDS_IN_AN_HOUR + min * SECONDS_IN_A_MINUTE;
  }

  REGOTH_DEFINE_RTTI(GameClock)
}  // namespace REGoth
