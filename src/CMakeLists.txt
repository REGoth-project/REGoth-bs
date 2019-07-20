add_library(samples-common
  common/BsCameraZoomer.h
  common/BsCameraZoomer.cpp
  common/BsFPSCamera.h
  common/BsFPSCamera.cpp
  common/BsFPSWalker.h
  common/BsFPSWalker.cpp
  common/BsObjectRotator.h
  common/BsObjectRotator.cpp
 )

target_include_directories(samples-common PUBLIC common)
target_link_libraries(samples-common PUBLIC bsf)

add_library(REGothEngine STATIC
  components/AnchoredTextLabels.hpp
  components/AnchoredTextLabels.cpp
  components/CharacterState.hpp
  components/CharacterState.cpp
  components/VisualCharacter.hpp
  components/VisualCharacter.cpp
  components/VisualSkeletalAnimation.hpp
  components/VisualSkeletalAnimation.cpp
  components/VisualStaticMesh.hpp
  components/VisualStaticMesh.cpp
  components/VisualInteractiveObject.hpp
  components/VisualInteractiveObject.cpp
  components/NodeVisuals.hpp
  components/NodeVisuals.cpp
  components/ScriptBackedBy.hpp
  components/ScriptBackedBy.cpp
  components/GameWorld.hpp
  components/GameWorld.cpp
  components/NeedsGameWorld.hpp
  components/NeedsGameWorld.cpp
  components/Visual.hpp
  components/Visual.cpp
  components/VisualMorphMesh.hpp
  components/VisualMorphMesh.cpp
  components/Item.hpp
  components/Item.cpp
  components/Character.hpp
  components/Character.cpp
  components/CharacterAI.hpp
  components/CharacterAI.cpp
  components/Waypoint.hpp
  components/Waypoint.cpp
  components/Freepoint.hpp
  components/Freepoint.cpp
  components/Waynet.hpp
  components/Waynet.cpp
  components/CharacterKeyboardInput.hpp
  components/CharacterKeyboardInput.cpp
  components/GameClock.hpp
  components/GameClock.cpp
  components/ThirdPersonCamera.hpp
  components/ThirdPersonCamera.cpp
  components/EventQueue.hpp
  components/EventQueue.cpp
  components/CharacterEventQueue.hpp
  components/CharacterEventQueue.cpp
  components/Sky.hpp
  components/Sky.cpp
  core.hpp
  core/EmptyGame.hpp
  core/EmptyGame.cpp
  core/Engine.hpp
  core/Engine.cpp
  core/EngineConfig.hpp
  core/EngineConfig.cpp
  core/ParseArguments.hpp
  core/ParseArguments.tpp
  core/RunEngine.hpp
  core/RunEngine.cpp
  components/Inventory.hpp
  components/Inventory.cpp
  sky/SkyColoring.hpp
  sky/SkyColoring.cpp
  components/Focusable.hpp
  components/Focusable.cpp
  components/StoryInformation.hpp
  components/StoryInformation.cpp
  components/GameplayUI.hpp
  components/GameplayUI.cpp
  components/UIElement.hpp
  components/UIElement.cpp
  components/UIFocusText.hpp
  components/UIFocusText.cpp
  components/UISubtitleBox.hpp
  components/UISubtitleBox.cpp
  components/UIDialogueChoice.hpp
  components/UIDialogueChoice.cpp
  AI/EventMessage.hpp
  AI/EventMessage.cpp
  AI/ScriptState.hpp
  AI/ScriptState.cpp
  AI/Pathfinder.hpp
  AI/Pathfinder.cpp
  exception/Throw.hpp
  animation/StateNaming.hpp
  animation/StateNaming.cpp
  animation/Animation.hpp
  animation/Animation.cpp
  scripting/daedalus/REGothDaedalusVM.hpp
  scripting/daedalus/REGothDaedalusVM.cpp
  scripting/daedalus/DATSymbolStorageLoader.hpp
  scripting/daedalus/DATSymbolStorageLoader.cpp
  scripting/daedalus/DaedalusClassVarResolver.hpp
  scripting/daedalus/DaedalusClassVarResolver.cpp
  scripting/daedalus/DaedalusStack.hpp
  scripting/daedalus/DaedalusStack.cpp
  scripting/daedalus/DaedalusDisassembler.hpp
  scripting/daedalus/DaedalusDisassembler.cpp
  scripting/daedalus/DaedalusVMForGameWorld.hpp
  scripting/daedalus/DaedalusVMForGameWorld.cpp
  scripting/ScriptSymbols.hpp
  scripting/ScriptSymbols.cpp
  scripting/ScriptSymbolStorage.hpp
  scripting/ScriptSymbolStorage.cpp
  scripting/ScriptObject.hpp
  scripting/ScriptObject.cpp
  scripting/ScriptObjectStorage.hpp
  scripting/ScriptObjectStorage.cpp
  scripting/ScriptVM.hpp
  scripting/ScriptVM.cpp
  scripting/ScriptSymbolQueries.hpp
  scripting/ScriptSymbolQueries.cpp
  scripting/ScriptClassTemplates.hpp
  scripting/ScriptClassTemplates.cpp
  scripting/ScriptVMForGameWorld.hpp
  scripting/ScriptVMForGameWorld.cpp
  scripting/ScriptObjectMapping.hpp
  scripting/ScriptObjectMapping.cpp
  original-content/VirtualFileSystem.hpp
  original-content/VirtualFileSystem.cpp
  original-content/OriginalGameFiles.hpp
  original-content/OriginalGameFiles.cpp
  original-content/OriginalGameResources.hpp
  original-content/OriginalGameResources.cpp
  engine-content/EngineContent.cpp
  engine-content/EngineContent.hpp
  engine-content/internal/FindEngineContent.cpp
  engine-content/internal/FindEngineContent.hpp
  world/internals/ConstructFromZEN.hpp
  world/internals/ConstructFromZEN.cpp
  gui/skin_gothic.hpp
  gui/skin_gothic.cpp
  world/internals/ImportSingleVob.hpp
  world/internals/ImportSingleVob.cpp
  RTTI/RTTI_CharacterAI.hpp
  RTTI/RTTI_Character.hpp
  RTTI/RTTI_CharacterKeyboardInput.hpp
  RTTI/RTTI_ScriptBackedBy.hpp
  RTTI/RTTI_ScriptObject.hpp
  RTTI/RTTI_ScriptObjectStorage.hpp
  RTTI/RTTI_TypeIDs.hpp
  RTTI/RTTIUtil.hpp
  RTTI/RTTI_VisualCharacter.hpp
  RTTI/RTTI_VisualStaticMesh.hpp
  RTTI/RTTI_VisualInteractiveObject.hpp
  RTTI/RTTI_VisualSkeletalAnimation.hpp
  RTTI/RTTI_NodeVisuals.hpp
  RTTI/RTTI_Waynet.hpp
  RTTI/RTTI_Waypoint.hpp
  RTTI/RTTI_GameClock.hpp
  RTTI/RTTI_UIElement.hpp
  RTTI/RTTI_UIFocusText.hpp
  RTTI/RTTI_StoryInformation.hpp
  RTTI/RTTI_Inventory.hpp
  )

target_link_libraries(REGothEngine PUBLIC bsf BsZenLib)
target_link_libraries(REGothEngine PUBLIC ${OpenMP_CXX_LIBRARIES})
target_link_libraries(REGothEngine PUBLIC cxxopts)
target_include_directories(REGothEngine PUBLIC .)

# Let CMake take care of the __declspec nonsense on windows.
set_target_properties(REGothEngine PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE) 

# Make sure our calls to BS_LOG work
target_compile_definitions(REGothEngine PUBLIC -DBS_LOG_VERBOSITY=LogVerbosity::Log)

# add_executable(REGoth main.cpp)
# target_link_libraries(REGoth REGothEngine samples-common)

add_executable(REGothCharacterViewer main_CharacterViewer.cpp)
target_link_libraries(REGothCharacterViewer REGothEngine samples-common)

add_executable(REGothWorldViewer main_WorldViewer.cpp)
target_link_libraries(REGothWorldViewer REGothEngine samples-common)

add_executable(REGothWorldMeshViewer main_WorldMeshViewer.cpp)
target_link_libraries(REGothWorldMeshViewer REGothEngine samples-common)

add_executable(REGothFontViewer main_FontViewer.cpp)
target_link_libraries(REGothFontViewer REGothEngine samples-common)

add_executable(REGothScriptTester main_ScriptTest.cpp)
target_link_libraries(REGothScriptTester REGothEngine samples-common)

add_executable(REGothWaynetTester main_WaynetTest.cpp)
target_link_libraries(REGothWaynetTester REGothEngine samples-common)

add_executable(REGothCharacterMovementTester main_CharacterMovementTest.cpp)
target_link_libraries(REGothCharacterMovementTester REGothEngine samples-common)

add_executable(REGothMobViewer main_MobViewer.cpp)
target_link_libraries(REGothMobViewer REGothEngine samples-common)

add_executable(REGothWorldCacheTest main_WorldCacheTest.cpp)
target_link_libraries(REGothWorldCacheTest REGothEngine samples-common)

add_executable(REGothFocusTester main_FocusTester.cpp)
target_link_libraries(REGothFocusTester REGothEngine samples-common)
