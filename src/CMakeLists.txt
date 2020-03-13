add_library(samples-common
  common/BsCameraZoomer.cpp
  common/BsCameraZoomer.h
  common/BsFPSCamera.cpp
  common/BsFPSCamera.h
  common/BsFPSWalker.cpp
  common/BsFPSWalker.h
  common/BsObjectRotator.cpp
  common/BsObjectRotator.h
 )

target_include_directories(samples-common PUBLIC common)
target_link_libraries(samples-common PUBLIC bsf)

add_library(REGothEngine STATIC
  AI/EventMessage.cpp
  AI/EventMessage.hpp
  AI/Pathfinder.cpp
  AI/Pathfinder.hpp
  AI/ScriptState.cpp
  AI/ScriptState.hpp
  RTTI/RTTIUtil.hpp
  RTTI/RTTI_Character.hpp
  RTTI/RTTI_CharacterAI.hpp
  RTTI/RTTI_CharacterKeyboardInput.hpp
  RTTI/RTTI_GameClock.hpp
  RTTI/RTTI_Inventory.hpp
  RTTI/RTTI_NodeVisuals.hpp
  RTTI/RTTI_ScriptBackedBy.hpp
  RTTI/RTTI_ScriptObject.hpp
  RTTI/RTTI_ScriptObjectStorage.hpp
  RTTI/RTTI_StoryInformation.hpp
  RTTI/RTTI_TypeIDs.hpp
  RTTI/RTTI_UIElement.hpp
  RTTI/RTTI_UIFocusText.hpp
  RTTI/RTTI_VisualCharacter.hpp
  RTTI/RTTI_VisualInteractiveObject.hpp
  RTTI/RTTI_VisualSkeletalAnimation.hpp
  RTTI/RTTI_VisualStaticMesh.hpp
  RTTI/RTTI_Waynet.hpp
  RTTI/RTTI_Waypoint.hpp
  animation/Animation.cpp
  animation/Animation.hpp
  animation/StateNaming.cpp
  animation/StateNaming.hpp
  components/AnchoredTextLabels.cpp
  components/AnchoredTextLabels.hpp
  components/Character.cpp
  components/Character.hpp
  components/CharacterAI.cpp
  components/CharacterAI.hpp
  components/CharacterEventQueue.cpp
  components/CharacterEventQueue.hpp
  components/CharacterKeyboardInput.cpp
  components/CharacterKeyboardInput.hpp
  components/CharacterState.cpp
  components/CharacterState.hpp
  components/EventQueue.cpp
  components/EventQueue.hpp
  components/Focusable.cpp
  components/Focusable.hpp
  components/Freepoint.cpp
  components/Freepoint.hpp
  components/GameClock.cpp
  components/GameClock.hpp
  components/GameWorld.cpp
  components/GameWorld.hpp
  components/GameplayUI.cpp
  components/GameplayUI.hpp
  components/Inventory.cpp
  components/Inventory.hpp
  components/Item.cpp
  components/Item.hpp
  components/NeedsGameWorld.cpp
  components/NeedsGameWorld.hpp
  components/NodeVisuals.cpp
  components/NodeVisuals.hpp
  components/ScriptBackedBy.cpp
  components/ScriptBackedBy.hpp
  components/Sky.cpp
  components/Sky.hpp
  components/SkyStateGenerator.cpp
  components/SkyStateGenerator.hpp
  components/StoryInformation.cpp
  components/StoryInformation.hpp
  components/ThirdPersonCamera.cpp
  components/ThirdPersonCamera.hpp
  components/UIDialogueChoice.cpp
  components/UIDialogueChoice.hpp
  components/UIElement.cpp
  components/UIElement.hpp
  components/UIFocusText.cpp
  components/UIFocusText.hpp
  components/UIInventory.cpp
  components/UIInventory.hpp
  components/UISubtitleBox.cpp
  components/UISubtitleBox.hpp
  components/Visual.cpp
  components/Visual.hpp
  components/VisualCharacter.cpp
  components/VisualCharacter.hpp
  components/VisualInteractiveObject.cpp
  components/VisualInteractiveObject.hpp
  components/VisualMorphMesh.cpp
  components/VisualMorphMesh.hpp
  components/VisualSkeletalAnimation.cpp
  components/VisualSkeletalAnimation.hpp
  components/VisualStaticMesh.cpp
  components/VisualStaticMesh.hpp
  components/Waynet.cpp
  components/Waynet.hpp
  components/Waypoint.cpp
  components/Waypoint.hpp
  core.hpp
  core/EmptyGame.cpp
  core/EmptyGame.hpp
  core/Engine.cpp
  core/Engine.hpp
  core/EngineConfig.cpp
  core/EngineConfig.hpp
  core/GameType.hpp
  core/Gothic1Game.cpp
  core/Gothic1Game.hpp
  core/Gothic2Game.cpp
  core/Gothic2Game.hpp
  core/ParseArguments.hpp
  core/ParseArguments.tpp
  core/RunEngine.cpp
  core/RunEngine.hpp
  engine-content/EngineContent.cpp
  engine-content/EngineContent.hpp
  engine-content/internal/FindEngineContent.cpp
  engine-content/internal/FindEngineContent.hpp
  exception/Assert.hpp
  exception/AssertionException.hpp
  exception/Throw.hpp
  gui/skin_gothic.cpp
  gui/skin_gothic.hpp
  original-content/OriginalGameFiles.cpp
  original-content/OriginalGameFiles.hpp
  original-content/OriginalGameResources.cpp
  original-content/OriginalGameResources.hpp
  original-content/VirtualFileSystem.cpp
  original-content/VirtualFileSystem.hpp
  scripting/ScriptClassTemplates.cpp
  scripting/ScriptClassTemplates.hpp
  scripting/ScriptObject.cpp
  scripting/ScriptObject.hpp
  scripting/ScriptObjectMapping.cpp
  scripting/ScriptObjectMapping.hpp
  scripting/ScriptObjectStorage.cpp
  scripting/ScriptObjectStorage.hpp
  scripting/ScriptSymbolQueries.cpp
  scripting/ScriptSymbolQueries.hpp
  scripting/ScriptSymbolStorage.cpp
  scripting/ScriptSymbolStorage.hpp
  scripting/ScriptSymbols.cpp
  scripting/ScriptSymbols.hpp
  scripting/ScriptVM.cpp
  scripting/ScriptVM.hpp
  scripting/ScriptVMForGameWorld.cpp
  scripting/ScriptVMForGameWorld.hpp
  scripting/daedalus/DATSymbolStorageLoader.cpp
  scripting/daedalus/DATSymbolStorageLoader.hpp
  scripting/daedalus/DaedalusClassVarResolver.cpp
  scripting/daedalus/DaedalusClassVarResolver.hpp
  scripting/daedalus/DaedalusDisassembler.cpp
  scripting/daedalus/DaedalusDisassembler.hpp
  scripting/daedalus/DaedalusStack.cpp
  scripting/daedalus/DaedalusStack.hpp
  scripting/daedalus/DaedalusVMForGameWorld.cpp
  scripting/daedalus/DaedalusVMForGameWorld.hpp
  scripting/daedalus/REGothDaedalusVM.cpp
  scripting/daedalus/REGothDaedalusVM.hpp
  world/internals/ConstructFromZEN.cpp
  world/internals/ConstructFromZEN.hpp
  world/internals/ImportSingleVob.cpp
  world/internals/ImportSingleVob.hpp
  )

# link necessary libraries for physfs on macOS
if(APPLE)
target_link_libraries(REGothEngine PUBLIC "-framework CoreFoundation")
target_link_libraries(REGothEngine PUBLIC "-framework Foundation")
target_link_libraries(REGothEngine PUBLIC "-framework IOKit")
endif(APPLE)

target_link_libraries(REGothEngine PUBLIC bsf BsZenLib)
target_link_libraries(REGothEngine PUBLIC ${OpenMP_CXX_LIBRARIES})
target_link_libraries(REGothEngine PUBLIC cxxopts)
target_include_directories(REGothEngine PUBLIC .)

# Let CMake take care of the __declspec nonsense on windows.
set_target_properties(REGothEngine PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

# Make sure our calls to BS_LOG work
target_compile_definitions(REGothEngine PUBLIC -DBS_LOG_VERBOSITY=LogVerbosity::Log)

add_executable(REGoth main.cpp)
target_link_libraries(REGoth REGothEngine samples-common)

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
