#include "StoryInformation.hpp"
#include <RTTI/RTTI_StoryInformation.hpp>
#include <components/Character.hpp>
#include <components/GameWorld.hpp>
#include <exception/Throw.hpp>
#include <scripting/ScriptSymbolQueries.hpp>
#include <scripting/ScriptVMForGameWorld.hpp>

namespace REGoth
{
  StoryInformation::StoryInformation(const bs::HSceneObject& parent, HGameWorld gameWorld,
                                     HCharacter self)
      : bs::Component(parent)
      , mGameWorld(gameWorld)
      , mSelf(self)
  {
    setName("StoryInformation");
  }

  StoryInformation::~StoryInformation()
  {
  }

  void StoryInformation::onInitialized()
  {
    if (mAllInfos.empty())
    {
      findAllInfos();
    }
  }

  void StoryInformation::findAllInfos()
  {
    auto& objectStorage = mGameWorld->scriptVM().scriptObjects();
    auto& symbolStorage = mGameWorld->scriptVM().scriptSymbols();

    for (Scripting::ScriptObjectHandle h : mSelf->allInfosForThisCharacter())
    {
      auto& data = objectStorage.get(h);

      DialogueInfo info;
      info.name        = data.instanceName;
      info.priority    = data.intValue("NR");
      info.isPermanent = data.intValue("PERMANENT") != 0;
      info.isImportant = data.intValue("IMPORTANT") != 0;
      info.isTrade     = data.intValue("TRADE") != 0;
      info.choiceText  = data.stringValue("DESCRIPTION");

      info.conditionFunction =
          symbolStorage.findFunctionByAddress(data.functionPointerValue("CONDITION"));
      info.informationFunction =
          symbolStorage.findFunctionByAddress(data.functionPointerValue("INFORMATION"));

      bs::UINT32 index = mAllInfos.size();

      mAllInfos.emplace_back(info);

      bs::gDebug().logDebug("[StoryInformation] Info " + info.name + " for " + mSelf->getName());
    }
  }

  bs::Vector<const StoryInformation::DialogueInfo*> StoryInformation::gatherAvailableDialogueLines(
      HCharacter other) const
  {
    HStoryInformation otherInfo = other->SO()->getComponent<StoryInformation>();

    bs::Vector<const StoryInformation::DialogueInfo*> result;

    for (bs::UINT32 i = 0; i < (bs::UINT32)mAllInfos.size(); i++)
    {
      if (isDialogueInfoAvaliable(i, other, otherInfo))
      {
        result.push_back(&mAllInfos[i]);
      }
    }

    return result;
  }

  bool StoryInformation::isDialogueInfoAvaliable(bs::UINT32 index, HCharacter other,
                                                 HStoryInformation otherInfo) const
  {
    const auto& info = mAllInfos[index];

    if (!info.isPermanent && otherInfo->knowsInfo(info.name))
    {
      return false;
    }

    return mGameWorld->scriptVM().runInfoConditionFunction(info.conditionFunction, mSelf, other);
  }

  bool StoryInformation::knowsInfo(const bs::String& name) const
  {
    return mKnownInfos.find(name) != mKnownInfos.end();
  }

  void StoryInformation::giveKnowledgeAboutInfo(const bs::String& name)
  {
    mKnownInfos.insert(name);
  }

  REGOTH_DEFINE_RTTI(StoryInformation)

}  // namespace REGoth
