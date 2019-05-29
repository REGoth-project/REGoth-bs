#include "StateNaming.hpp"
#include <exception/Throw.hpp>

using namespace REGoth;

const AI::WeaponMode ALL_WEAPON_MODES[] = {
    AI::WeaponMode::None,      AI::WeaponMode::OneHanded,
    AI::WeaponMode::TwoHanded, AI::WeaponMode::Bow,
    AI::WeaponMode::Crossbow,  AI::WeaponMode::Magic,
    AI::WeaponMode::Fist,
};

static bs::String getWeaponAniTag(AI::WeaponMode weapon)
{
  using namespace AI;

  switch (weapon)
  {
    case WeaponMode::None:
      return "";
    case WeaponMode::OneHanded:
      return "1H";
    case WeaponMode::TwoHanded:
      return "2H";
    case WeaponMode::Bow:
      return "BOW";
    case WeaponMode::Crossbow:
      return "CBOW";
    case WeaponMode::Magic:
      return "MAG";
    case WeaponMode::Fist:
      return "FIST";
    default:
      REGOTH_THROW(InvalidStateException, "Invalid weapon type");
  }
}

static bs::String getWalkModeAniTag(AI::WalkMode walkMode)
{
  using namespace AI;

  switch (walkMode)
  {
    case WalkMode::Run:
      return "RUN";
    case WalkMode::Walk:
      return "WALK";
    case WalkMode::Sneak:
      return "SNEAK";
    case WalkMode::Water:
      return "WATER";
    case WalkMode::Swim:
      return "SWIM";
    case WalkMode::Dive:
      return "DIVE";
    default:
      REGOTH_THROW(InvalidStateException, "Invalid WalkMode");
  }
}

bs::String AnimationState::constructStateAnimationName(AI::WeaponMode weaponMode,
                                                       AI::WalkMode walkMode, const bs::String& name)
{
  return "S_" + getWeaponAniTag(weaponMode) + getWalkModeAniTag(walkMode) + name;
}

bs::String AnimationState::constructTransitionAnimationName(AI::WeaponMode weaponMode,
                                                            const bs::String& from,
                                                            const bs::String& to)
{
  const bs::String weaponAniTag = getWeaponAniTag(weaponMode);

  return "T_" + weaponAniTag + from + "_2_" + weaponAniTag + to;
}

bs::String AnimationState::getStateName(const bs::String& animation)
{
  if (animation.length() < 3) return "";
  if (animation[0] != 'S') return "";
  if (animation[1] != '_') return "";

  bs::String noTag = animation.substr(2);

  return stripWeaponModeFromAnimationName(noTag);
}

AI::WeaponMode AnimationState::getWeaponMode(const bs::String& animation)
{
  if (animation.length() < 3) return AI::WeaponMode::None;
  if (animation[0] != 'S') return AI::WeaponMode::None;
  if (animation[1] != '_') return AI::WeaponMode::None;

  for (AI::WeaponMode m : ALL_WEAPON_MODES)
  {
    bs::String weaponModeTag = getWeaponAniTag(m);

    if (animation.substr(2, weaponModeTag.length()) == weaponModeTag)
    {
      return m;
    }
  }

  return AI::WeaponMode::None;
}

bs::String AnimationState::stripWeaponModeFromAnimationName(const bs::String& animation)
{
  for (AI::WeaponMode m : ALL_WEAPON_MODES)
  {
    bs::String weaponModeTag = getWeaponAniTag(m);

    if (animation.substr(0, weaponModeTag.length()) == weaponModeTag)
    {
      return animation.substr(weaponModeTag.length());
    }
  }

  return animation;
}

bool AnimationState::isTransitionNeeded(const bs::String& animation)
{
  if (animation.length() < 3) return true;
  if (animation[0] != 'T') return true;
  if (animation[1] != '_') return true;

  return false;
}
