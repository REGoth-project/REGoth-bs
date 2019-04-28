#include "GameClock.hpp"
#include <RTTI/RTTI_GameClock.hpp>
#include <Scene/BsSceneObject.h>
#include <exception/Throw.hpp>
#include <Math/BsMath.h>

namespace REGoth
{
  constexpr float SECONDS_IN_A_MINUTE = 60;
  constexpr float SECONDS_IN_AN_HOUR  = 60 * SECONDS_IN_A_MINUTE;
  constexpr float SECONDS_IN_A_DAY    = 24 * SECONDS_IN_AN_HOUR;
  constexpr float CLOCK_SPEED_FACTOR  = 14.5;

  GameClock::GameClock(const bs::HSceneObject& parent)
    : bs::Component(parent)
  { }

  void GameClock::update()
  {
    float delta = bs::gTime().getFrameDelta();

    mElapsedSeconds       += delta;
    mElapsedIngameSeconds += delta * CLOCK_SPEED_FACTOR;
  }

  bs::UINT32 GameClock::getDay() const
  {
    return bs::Math::floorToPosInt(mElapsedIngameSeconds/SECONDS_IN_A_DAY);
  }

  bs::UINT8 GameClock::getHour() const
  {
    float elapsedSecondsOfDay = mElapsedIngameSeconds-(getDay()*SECONDS_IN_A_DAY);
    
    return bs::Math::floorToPosInt(elapsedSecondsOfDay/SECONDS_IN_AN_HOUR);
  }

  bs::UINT8 GameClock::getMinute() const
  {
    float elapsedSecondsOfHour = mElapsedIngameSeconds-(getDay()*SECONDS_IN_A_DAY)-(getHour()*SECONDS_IN_AN_HOUR);

    return bs::Math::floorToPosInt(elapsedSecondsOfHour/SECONDS_IN_A_MINUTE);
  }

  bool GameClock::isTime(bs::UINT8 hour1, bs::UINT8 min1, bs::UINT8 hour2, bs::UINT8 min2) const
  {
    float elapsedSecondsOfDay = mElapsedIngameSeconds-(getDay()*SECONDS_IN_A_DAY);
    float lowerElapsedSecondsOfDay = ((hour1*SECONDS_IN_AN_HOUR)+(min1*SECONDS_IN_A_MINUTE))*CLOCK_SPEED_FACTOR;
    float upperElapsedSecondsOfDay = ((hour2*SECONDS_IN_AN_HOUR)+(min2*SECONDS_IN_A_MINUTE))*CLOCK_SPEED_FACTOR;

    // TODO: Not sure if Gothic allows the order of the input times to be reversed
    return (elapsedSecondsOfDay>lowerElapsedSecondsOfDay && elapsedSecondsOfDay<upperElapsedSecondsOfDay);
  }

  void GameClock::setTime(bs::UINT8 hour, bs::UINT8 min)
  {
    bs::INT8 minutesToAdvance = (min%60)-getMinute();
    bs::INT8 hoursToAdvance = (hour%24)-getHour();
    bs::UINT8 daysToAdvance = hour/24;

    mElapsedIngameSeconds += ((daysToAdvance*SECONDS_IN_A_DAY)+(hoursToAdvance*SECONDS_IN_AN_HOUR)+(minutesToAdvance*SECONDS_IN_A_MINUTE))*CLOCK_SPEED_FACTOR;
  }

  bs::RTTITypeBase* GameClock::getRTTIStatic()
  {
    return RTTI_GameClock::instance();
  }

  bs::RTTITypeBase* GameClock::getRTTI() const
  {
    return GameClock::getRTTIStatic();
  }
}  // namespace REGoth
