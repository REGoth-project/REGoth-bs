#include "Debug.hpp"
#include <RTTI/RTTI_Debug.hpp>
#include <log/logging.hpp>

namespace REGoth
{
  Debug::Debug(const bs::HSceneObject& parent)
      : bs::Component(parent)
  {
    setName("Debug");
  }

  Debug::~Debug()
  {
  }

  void Debug::onInitialized()
  {
    mFastMove = bs::VirtualButton("FastMove");
  }

  void Debug::update()
  {
    // TODO: get reference to playable charcter for various debug functions

    //FastMove
    if (bs::gVirtualInput().isButtonHeld(mFastMove))
    {
      REGOTH_LOG(Info, Uncategorized, "[Debug] FastMove Active!");
      //mCharacterAI->fastMove(4.0f);
    }
    else
    {
      //mCharacterAI->fastMove(1.0f);
    }
  }

  REGOTH_DEFINE_RTTI(Debug)
}  // namespace REGoth
