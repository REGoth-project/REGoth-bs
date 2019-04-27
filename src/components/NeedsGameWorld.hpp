#pragma once
#include <BsPrerequisites.h>
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  /**
   * Base-class for all components needing a GameWorld to work.
   */
  class NeedsGameWorld : public bs::Component
  {
  public:
    NeedsGameWorld(const bs::HSceneObject& parent);
    virtual ~NeedsGameWorld();

  protected:
    /**
     * @return GameWorld this scene object is in.
     */
    HGameWorld gameWorld() const
    {
      return mGameWorld;
    }

  private:

    /**
     * Searches the scenes root for a GameWorld-Component.
     *
     * Throws if it cannot find one.
     */
    void findGameWorld();

    /**
     * Handle to the parent GameWorld-component.
     */
    HGameWorld mGameWorld;

  public:
    REGOTH_DECLARE_RTTI(NeedsGameWorld)

  public:                            // FIXME: RTTI, make protected
    NeedsGameWorld() = default;  // Serialization only
  };
}  // namespace REGoth
