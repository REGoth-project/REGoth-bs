#include "GameClock.hpp"
#include <RTTI/RTTI_GameClock.hpp>
#include <Scene/BsSceneObject.h>
#include <Math/BsMath.h>

namespace REGoth
{
  constexpr float SECONDS_IN_A_MINUTE = 60;
  constexpr float SECONDS_IN_AN_HOUR  = 60 * SECONDS_IN_A_MINUTE;
  constexpr float SECONDS_IN_A_DAY    = 24 * SECONDS_IN_AN_HOUR;
  constexpr float CLOCK_SPEED_FACTOR  = SECONDS_IN_A_DAY/6000; // see https://forum.worldofplayers.de/forum/threads/396326-Tipp-Tageweise-springen-%28auch-zur%C3%BCck%29?p=6231841&viewfull=1#post6231841

  GameClock::GameClock(const bs::HSceneObject& parent)
    : bs::Component(parent)
  { }

  void GameClock::fixedUpdate()
  {
    float delta = bs::gTime().getFixedFrameDelta();

    mElapsedSeconds       += delta;
    mElapsedIngameSeconds += delta * CLOCK_SPEED_FACTOR;
  }

  bs::INT32 GameClock::getDay() const
  {
    return bs::Math::floorToPosInt(mElapsedIngameSeconds / SECONDS_IN_A_DAY);
  }

  bs::INT32 GameClock::getHour() const
  {
    float elapsedSecondsOfDay = mElapsedIngameSeconds - (getDay() * SECONDS_IN_A_DAY);
    
    return bs::Math::floorToPosInt(elapsedSecondsOfDay / SECONDS_IN_AN_HOUR);
  }

  bs::INT32 GameClock::getMinute() const
  {
    float elapsedSecondsOfHour = mElapsedIngameSeconds - (getDay() * SECONDS_IN_A_DAY) - (getHour() * SECONDS_IN_AN_HOUR);

    return bs::Math::floorToPosInt(elapsedSecondsOfHour / SECONDS_IN_A_MINUTE);
  }

  bool GameClock::isTime(bs::INT32 hour1, bs::INT32 min1, bs::INT32 hour2, bs::INT32 min2) const
  {
    // TODO: Do we need to handle negative input parameters smaller than -24?
    hour1 = (hour1 + 24) % 24;
    min1  = (min1  + 60) % 60;
    hour2 = (hour2 + 24) % 24;
    min2  = (min2  + 60) % 60;

    float currentTimeOfDayInSeconds = mElapsedIngameSeconds - (getDay() * SECONDS_IN_A_DAY);
    float firstTimeOfDayInSeconds   = (hour1*SECONDS_IN_AN_HOUR + min1*SECONDS_IN_A_MINUTE);
    float secondTimeOfDayInSeconds  = (hour2*SECONDS_IN_AN_HOUR + min2*SECONDS_IN_A_MINUTE);

    if (firstTimeOfDayInSeconds != secondTimeOfDayInSeconds)
    {
      secondTimeOfDayInSeconds -= 60.0; // Gothic subtracts one minute from the second time here to counter issues happening with overlapping times for Daily Routines. - markusobi
    }

    bool retval;
    if ( hour1 == hour2 && min1 == min2)
    {
      retval = false;
    }
    else if (firstTimeOfDayInSeconds > secondTimeOfDayInSeconds)
    {
      retval = (currentTimeOfDayInSeconds >= firstTimeOfDayInSeconds) || (currentTimeOfDayInSeconds <= secondTimeOfDayInSeconds);
    }
    else /* firstTimeOfDayInSeconds < secondTimeOfDayInSeconds */
    {
      retval = (currentTimeOfDayInSeconds >= firstTimeOfDayInSeconds) && (currentTimeOfDayInSeconds <= secondTimeOfDayInSeconds);
    }

    return retval;
  }

  void GameClock::setTime(bs::INT32 hour, bs::INT32 min)
  {
    // TODO: Negative input parameters are supported for hour, but what about negative values that result in negative remaining hours?
    //       Do we need to handle negative Minutes at all?
    float elapsedDaysInSeconds = getDay() * SECONDS_IN_A_DAY;
    bs::INT32 daysToAdvance    = hour / 24;

    hour = hour % 24;
    min  = min % 60;

    mElapsedIngameSeconds = elapsedDaysInSeconds + (daysToAdvance*SECONDS_IN_A_DAY + hour*SECONDS_IN_AN_HOUR + min*SECONDS_IN_A_MINUTE);

    // TODO: According to https://forum.worldofplayers.de/forum/threads/396326-Tipp-Tageweise-springen-%28auch-zur%C3%BCck%29?p=6231841&viewfull=1#post6231841
    //       if this shall be the Wld_setTime external, it also needs to implement these three functions here
    //       RoutineManager.SetDailyRoutinePos(RoutinesOnly)
    //       Game.SetObjectRoutineTimeChange(GameHour, GameMinute, Hour, Minute)
    //       SpawnManager.SpawnImmediately(ResetSpawnTime)
  }

  REGOTH_DEFINE_RTTI(GameClock)
}  // namespace REGoth
