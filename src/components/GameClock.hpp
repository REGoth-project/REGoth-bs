#pragma once
#include <BsPrerequisites.h>
#include <Scene/BsComponent.h>
#include <Utility/BsTime.h>
#include <RTTI/RTTIUtil.hpp>

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
     * @return  Whether the current ingame time of day (hh::mm) is between the two given times of day.
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

  private:
    float mElapsedSeconds       = 0.0;
    float mElapsedIngameSeconds = 0.0;

  /************************************************************************/
  /* RTTI                                                                 */
  /************************************************************************/
  public:
    REGOTH_DECLARE_RTTI(GameClock)

  // protected:
  public:  // FIXME: Should be protected, it is only used by RTTI but friend doesn't seem to work?!
    GameClock() = default;  // Serialization only
  };

  using HGameClock = bs::GameObjectHandle<GameClock>;
}  // namespace REGoth
