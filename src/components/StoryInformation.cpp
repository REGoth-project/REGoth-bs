#include "StoryInformation.hpp"
#include <RTTI/RTTI_StoryInformation.hpp>
#include <components/Character.hpp>
#include <components/GameWorld.hpp>
#include <exception/Throw.hpp>
#include <log/logging.hpp>
#include <scripting/ScriptSymbolQueries.hpp>
#include <scripting/ScriptVMForGameWorld.hpp>

// TODO: Refactor, so we don't access deep into the UI code here for dialogues
#include <components/GameplayUI.hpp>
#include <components/UIDialogueChoice.hpp>

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

      REGOTH_LOG(Info, Uncategorized, "[StoryInformation] Info {0} for {1}", info.name,
                 mSelf->getName());
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

  void StoryInformation::startDialogueWith(HCharacter other)
  {
    gGameplayUI()->startDialogue();

    for (auto info : gatherAvailableDialogueLines(other))
    {
      addChoice(info->name, info->choiceText, info->informationFunction);
    }

    gGameplayUI()->choices()->setOnChoiceCallback([this, other](UIDialogueChoice::Choice choice) {
      REGOTH_LOG(Info, Uncategorized, "[StoryInformation] Choice taken: {0} ({1})", choice.text,
                 choice.instanceName);

      if (!choice.instanceName.empty())
      {
        HStoryInformation otherInfo = other->SO()->getComponent<StoryInformation>();

        otherInfo->giveKnowledgeAboutInfo(choice.instanceName);
      }

      // Re-fill the choices window before executing the script because it may call
      // `Info_ClearChoices` to supply some custom choices.
      clearChoices();

      for (auto info : gatherAvailableDialogueLines(other))
      {
        addChoice(info->name, info->choiceText, info->informationFunction);
      }

      mGameWorld->scriptVM().runInfoFunction(choice.scriptFunction, mSelf, other);
    });
  }

  void StoryInformation::stopDialogueWith(HCharacter other)
  {
    gGameplayUI()->stopDialogue();
  }

  void StoryInformation::addChoice(const bs::String& instanceName, const bs::String& text,
                                   Scripting::SymbolIndex infoFunction)
  {
    UIDialogueChoice::Choice c;
    c.instanceName   = instanceName;
    c.text           = text;
    c.scriptFunction = infoFunction;

    gGameplayUI()->choices()->addChoice(c);
  }

  void StoryInformation::clearChoices()
  {
    gGameplayUI()->choices()->clearChoices();
  }

  REGOTH_DEFINE_RTTI(StoryInformation)

}  // namespace REGoth
