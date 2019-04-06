#include "CharacterState.hpp"

namespace REGoth
{
  CharacterState::CharacterState(const bs::HSceneObject& parent)
      : Component(parent)
  {
    setName("CharacterState");
  }

  void CharacterState::fixedUpdate()
  {
    
  }
}
