#include "StateNaming.hpp"

using namespace REGoth;

const Animation::WeaponMode ALL_WEAPON_MODES[] = {
    Animation::WeaponMode::None, Animation::WeaponMode::OneHanded, Animation::WeaponMode::TwoHanded,
    Animation::WeaponMode::Bow,  Animation::WeaponMode::Crossbow,  Animation::WeaponMode::Magic,
    Animation::WeaponMode::Fist,
};

static bs::String getWeaponAniTag(Animation::WeaponMode weapon)
{
  using namespace Animation;
  using namespace bs;

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
      BS_EXCEPT(InvalidStateException, "Invalid weapon type");
  }
}

bs::String Animation::constructStateAnimationName(Animation::WeaponMode weaponMode,
                                                  const bs::String& name)
{
  return "S_" + getWeaponAniTag(weaponMode) + name;
}

bs::String Animation::constructTransitionAnimationName(Animation::WeaponMode weaponMode,
                                                       const bs::String& from, const bs::String& to)
{
  const bs::String weaponAniTag = getWeaponAniTag(weaponMode);

  return "T_" + weaponAniTag + from + "_2_" + weaponAniTag + to;
}

bs::String Animation::getStateName(const bs::String& animation)
{
  if (animation.length() < 3) return "";
  if (animation[0] != 'S') return "";
  if (animation[1] != '_') return "";

  bs::String noTag = animation.substr(2);

  return stripWeaponModeFromAnimationName(noTag);
}

Animation::WeaponMode Animation::getWeaponMode(const bs::String& animation)
{
  if (animation.length() < 3) return WeaponMode::None;
  if (animation[0] != 'S') return WeaponMode::None;
  if (animation[1] != '_') return WeaponMode::None;

  for (WeaponMode m : ALL_WEAPON_MODES)
  {
    bs::String weaponModeTag = getWeaponAniTag(m);

    if (animation.substr(2, weaponModeTag.length()) == weaponModeTag)
    {
      return m;
    }
  }

  return WeaponMode::None;
}

bs::String Animation::stripWeaponModeFromAnimationName(const bs::String& animation)
{
  for (WeaponMode m : ALL_WEAPON_MODES)
  {
    bs::String weaponModeTag = getWeaponAniTag(m);

    if (animation.substr(0, weaponModeTag.length()) == weaponModeTag)
    {
      return animation.substr(weaponModeTag.length());
    }
  }

  return animation;
}

bool Animation::isTransitionNeeded(const bs::String& animation)
{
  if (animation.length() < 3) return true;
  if (animation[0] != 'T') return true;
  if (animation[1] != '_') return true;

  return false;
}
