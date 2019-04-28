#pragma once
#include <BsPrerequisites.h>
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
     * Triggered once per frame. Updates elapsedSeconds for play and ingame time.
     */
    void update() override;

    /**
     * @return Current ingame clock day.
     */
    bs::UINT32 getDay() const;

    /**
     * @return Current ingame clock hour.
     */
    bs::UINT8 getHour() const;

    /**
     * @return Current ingame clock minute.
     */
    bs::UINT8 getMinute() const;

    /**
     * @return  Whether the current ingame clock (hh::mm) is between the two given values.
     *
     * @param   hour1
     *              Lower limit of elapsed ingame time in hours to check against.
     *              Is added to param min1.
     * @param   min1
     *              Lower limit of elapsed ingame time in minutes to check against.
     *              Is added to parameter hour1.
     * @param   hour2
     *              Upper limit of elapsed ingame time in hours to check against.
     *              Is added to param min2.     
     * @param   min2
     *              Upper limit of elapsed ingame time in minutes. to check against.
     *              Is added to parameter hour2.
     */
    bool isTime(bs::UINT8 hour1, bs::UINT8 min1, bs::UINT8 hour2, bs::UINT8 min2) const;
    
    /**
     * Sets the ingame clock to the given hh:mm.
     * Skips to the next occurence of the given hh::mm advancing the day.
     * 
     * @param  hour
     *             Can be a value over 23 to advance to the next day.
     * @param  min
     *             Minute.
     */
    void setTime(bs::UINT8 hour, bs::UINT8 min);

  private:
    float mElapsedSeconds       = 0.0;
    float mElapsedIngameSeconds = 0.0;

  /************************************************************************/
  /* RTTI                                                                 */
  /************************************************************************/
  public:
    friend class RTTI_CharacterKeyboardInput;
    static bs::RTTITypeBase* getRTTIStatic();
    bs::RTTITypeBase* getRTTI() const override;

  // protected:
  public:  // FIXME: Should be protected, it is only used by RTTI but friend doesn't seem to work?!
    GameClock() = default;  // Serialization only
  };

  using HGameClock = bs::GameObjectHandle<GameClock>;
}  // namespace REGoth
