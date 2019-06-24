#pragma once
#include "scripting/ScriptTypes.hpp"
#include <RTTI/RTTIUtil.hpp>
#include <Scene/BsComponent.h>

namespace REGoth
{
  class GameWorld;
  using HGameWorld = bs::GameObjectHandle<GameWorld>;

  class Character;
  using HCharacter = bs::GameObjectHandle<Character>;

  class StoryInformation;
  using HStoryInformation = bs::GameObjectHandle<StoryInformation>;

  /**
   * If you talk to an NPC and select a dialogue line for the hero to say, the
   * hero will remember the conversation. Most dialogue lines you can choose are
   * attached to an *Information*-Instance inside the games script files. This
   * *Information*-Instance holds the text to display in the UI, which script
   * function to execute once the dialogue line was chosen by the user via the
   * UI among some other information.
   *
   * The hero will simply store a list of all *Information*-Instances the user
   * has selected and talked about with the NPC. Actually, not only the hero can
   * store those *Information*-Instances he knows about but any other NPC can.
   * This is not used in the original game as far as I know, but it seems to be
   * a remnant of multiplayer functionality. Or you could have two NPCs talk to
   * each other.
   *
   * Since not all dialogue lines should be available to the user at all times,
   * there is a *Condition*-function for each *Information*-instance. Once the
   * player talks to an NPC and the available dialogue lines are evaluated,
   * their condition function is called. If it returns 0, the dialogue line is
   * not available. Otherwise it is shown to the user and they can select it.
   * This is used for story related dialogue lines which are only available
   * after a certain action has been done. For example:
   *
   *     FUNC INT Info_Thorus_DiegoSentMe_Condition()
   *     {
   *       if (   Npc_KnowsInfo(hero,Info_Thorus_WorkForGomez)
   *           && Npc_KnowsInfo(hero,Info_Diego_JoinOldcamp))
   *       {
   *         return 1;
   *       }
   *     }
   *
   * Usually, once you have talked about something, the dialogue line disappears
   * from the list of choices, since it often makes no sense to talk about the
   * same thing twice. If you want the dialogue line to always be visible, you
   * can set the `permanent` flag. Then, the dialogue line will always be shown
   * if the condition function allows it.
   *
   * An example for such an *Information*-instance would be:
   *
   *     INSTANCE Info_Thorus_WorkForGomez(C_INFO) //E2
   *     {
   *       npc         = GRD_200_THORUS;
   *       nr          = 3;
   *       condition   = Info_Thorus_WorkForGomez_Condition;
   *       information = Info_Thorus_WorkForGomez_Info;
   *       permanent   = 0;
   *       description = "I want to work for Gomez.";
   *     };
   *
   *
   * This component is attached to a character. It then stores a list of all
   * *Information*-instances this character can be asked about. And which have
   * the character has asked others about. It can also evaluate which dialogue
   * lines should be displayed when the hero talks to this character.
   *
   */
  class StoryInformation : public bs::Component
  {
  public:
    StoryInformation(const bs::HSceneObject& parent, HGameWorld gameWorld, HCharacter self);
    virtual ~StoryInformation();

    /**
     * Native version if the `C_INFO` script class for efficiency.
     */
    struct DialogueInfo
    {
      /**
       * Instance name of this info
       */
      bs::String name;

      /**
       * Called `nr` in the original script files. This defines the order the dialogue lines
       * should be displayed in the UI. However, sometimes those numbers are not unique, some are
       * missing, so `priority` is the better name here. Low numbers mean higher priority.
       */
      bs::UINT32 priority;

      /**
       * Script function to execute to check whether this information should be avaialble to the
       * user in the UI. If this returns 0, it should not be shown.
       */
      Scripting::SymbolIndex conditionFunction;

      /**
       * Script function to execute once the user has chosen this dialogue line. This usually
       * does the back-and-forth conversation.
       */
      Scripting::SymbolIndex informationFunction;

      /**
       * Whether this information should disapper once you talked about it once.
       */
      bool isPermanent;

      /**
       * If true, the NPC should start talking to the hero as soon as the hero comes into range.
       * This is often used for guards who are supposed to stop the player from entering
       * certain locations.
       */
      bool isImportant;

      /**
       * If true, this information will open the trade window.
       */
      bool isTrade;

      /**
       * The text to display to the user in the UI. Called `description` inside scripts.
       */
      bs::String choiceText;
    };

    /**
     * Assembles a list of all dialogue lines to be shown to the user in the UI.
     *
     * @param  other  Dialogue-Parter. Usually this is the hero.
     */
    bs::Vector<const DialogueInfo*> gatherAvailableDialogueLines(HCharacter other) const;

    /**
     * @return Whether the character knows the given info.
     *
     * @param  name  UPPERCASE Name of the *Information*-Instance, e.g. `INFO_THORUS_WORKFORGOMEZ`.
     *
     * Throws if this info is not available at all for this character.
     */
    bool knowsInfo(const bs::String& name) const;

    /**
     * Lets this character remember that someone talked about the given info with them.
     *
     * @param  name  UPPERCASE Name of the *Information*-Instance, e.g. `INFO_THORUS_WORKFORGOMEZ`.
     *
     * Throws if this info is not available at all for this character.
     */
    void giveKnowledgeAboutInfo(const bs::String& name);

    /**
     * TODO: Refactor, so this doesn't need to access the UI internally. I'd like this class to
     *       be more of a dumb thing to the *Information*-instances, rather than one that accesses
     *       the ui...
     *
     * Starts a dialogue with the given other character. If the dialogue is already going, it's
     * continuing it.
     */
    void startDialogueWith(HCharacter other);
    void stopDialogueWith(HCharacter other);

    /**
     * Adds a custom choice the the current choice list. (Used from script)
     */
    void addChoice(const bs::String& instanceName, const bs::String& text,
                   Scripting::SymbolIndex infoFunction);

    /**
     * Removes all choices from the choice list so that custom choices can be added (Used from
     * script).
     */
    void clearChoices();

  protected:
    void onInitialized() override;

  private:
    /**
     * Looks through all script symbols to find all *Information*-Instances available to this
     * character.
     */
    void findAllInfos();

    /**
     * @return Whether the given DialogueInfos dialogue line should be shown to the user in the UI.
     *
     * @param  index      Info-Index into mAllInfos.
     * @param  other      Dialogue-Parter. Usually this is the hero.
     * @param  otherInfo  StoryInformation component of `other`. Could be retrieved internally, but
     *                    is a parameter here for efficiency when calling this in a loop.
     *
     * @note   May call script code!
     */
    bool isDialogueInfoAvaliable(bs::UINT32 index, HCharacter other,
                                 HStoryInformation otherInfo) const;

  public:
    REGOTH_DECLARE_RTTI(StoryInformation)

    /** All possible infos to talk about with this character. These are static and
        can be gathered directly from the scripts on startup.
        Not serialized, can be generated from script symbols easily. */
    bs::Vector<DialogueInfo> mAllInfos;

    /** Set of *Information*-Instances this character knows. Contains names such as
       `INFO_THORUS_WORKFORGOMEZ` */
    bs::Set<bs::String> mKnownInfos;

    HCharacter mSelf;
    HGameWorld mGameWorld;

  protected:
    StoryInformation() = default;  // For RTTI
  };
}  // namespace REGoth
