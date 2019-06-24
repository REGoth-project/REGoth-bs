#pragma once

#include "RTTIUtil.hpp"
#include <scripting/daedalus/DaedalusVMForGameWorld.hpp>

namespace REGoth
{
  namespace Scripting
  {
    class RTTI_DaedalusVMForGameWorld
        : public bs::RTTIType<DaedalusVMForGameWorld, DaedalusVM, RTTI_DaedalusVMForGameWorld>
    {
      BS_BEGIN_RTTI_MEMBERS
      BS_RTTI_MEMBER_REFL(mWorld, 0)
      BS_RTTI_MEMBER_PLAIN(mHeroSymbol, 1)
      BS_RTTI_MEMBER_PLAIN(mSelfSymbol, 2)
      BS_RTTI_MEMBER_PLAIN(mOtherSymbol, 3)
      BS_RTTI_MEMBER_PLAIN(mVictimSymbol, 4)
      BS_RTTI_MEMBER_PLAIN(mItemSymbol, 5)
      BS_END_RTTI_MEMBERS

    public:
      RTTI_DaedalusVMForGameWorld()
      {
      }

      void onDeserializationEnded(bs::IReflectable* _obj, bs::SerializationContext* context) override
      {
        auto obj = static_cast<DaedalusVMForGameWorld*>(_obj);

        obj->createAllInformationInstances();
      }

      REGOTH_IMPLEMENT_RTTI_CLASS_FOR_REFLECTABLE(DaedalusVMForGameWorld)
    };
  }  // namespace Scripting

}  // namespace REGoth
