#pragma once
#include <BsPrerequisites.h>
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>
#include <Utility/BsTime.h>

namespace REGoth
{
  /**
   * Component that handles play and ingame time.
   * Offers externals for timespecific Wld_* functions.
   * Shall be instantiated with the World.
   */
  class GameClock : public bs::Component
  {
  public:
    GameClock(const bs::HSceneObject& parent);

    /**
     * Triggered once every fixed time step. Updates elapsedSeconds for play and ingame time.
     */
    void fixedUpdate() override;

    /**
     * @return Current ingame day.
     */
    bs::INT32 getDay() const;

    /**
     * @return Current hour of time of day (hh:mm).
     */
    bs::INT32 getHour() const;

    /**
     * @return Current Minute of time of day (hh:mm).
     */
    bs::INT32 getMinute() const;

    /**
     * @return Ratio of how far the current day has progressed.
     *         If the clock says 0:00, this will return 0.0f.
     *         IF the clock says 23:59, a value close to 1.0f will be returned.
     */
    float getDayRatio() const;

    /**
     * @return Number of seconds elapsed in the current day counted in ingame-time.
     *         This resets to 0 when a new day starts.
     *         The maximum value this will return is 60 * 60 * 24.
     */
    float getDaySeconds() const;

    /**
     * @return  Whether the current ingame time of day (hh::mm) is between the two given times of
     * day.
     *
     * @note    The first time of day can be after the second time of day.
     *
     * @param   hour1
     *              Hour of the first time of day (\p hour1 : \p min1 ) to check against.
     * @param   min1
     *              Minute of the first time of day (\p hour1 : \p min1 ) to check against.
     * @param   hour2
     *              Hour of the second time of day (\p hour2 : \p min2 ) to check against.
     * @param   min2
     *              Minute of the second time of day (\p hour2 : \p min2 ) to check against.
     */
    bool isTime(bs::INT32 hour1, bs::INT32 min1, bs::INT32 hour2, bs::INT32 min2) const;

    /**
     * Sets the ingame clock (hh:mm) to the given time in hour and minute.
     *
     * @note   Values for \p hour can be over 23 to advance days too.
     *         Calling setTime(24+15, 0) sets the clock to 15:00 and advances one day.
     *
     * @param  hour
     *             Accepts any value (int32), but will use \p hour%24 internally.
     *             Can also advance days - see note.
     * @param  min
     *             Accepts any value (int32), but will use \p min%60 internally.
     */
    void setTime(bs::INT32 hour, bs::INT32 min);

    void setDay(bs::UINT32 day);

  private:
    float mElapsedSeconds       = 0.0f;
    float mElapsedIngameSeconds = 0.0f;

    void setTime(bs::UINT32 day, bs::UINT8 hour, bs::UINT8 min);

  public:
    REGOTH_DECLARE_RTTI(GameClock)

  protected:
    GameClock() = default;  // Serialization only
  };

  using HGameClock = bs::GameObjectHandle<GameClock>;
}  // namespace REGoth
