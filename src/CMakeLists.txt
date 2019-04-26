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

add_library(REGothEngine
  REGothEngine.hpp
  REGothEngine.cpp
  components/AnchoredTextLabels.h
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
  components/StartSpot.hpp
  components/StartSpot.cpp
  components/Spot.hpp
  components/Spot.cpp
  components/NodeVisuals.hpp
  components/NodeVisuals.cpp
  components/ScriptBackedBy.hpp
  components/ScriptBackedBy.cpp
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
  components/Waynet.hpp
  components/Waynet.cpp
  components/CharacterKeyboardInput.hpp
  components/CharacterKeyboardInput.cpp
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
  scripting/daedalus/DaedalusVMWithExternals.hpp
  scripting/daedalus/DaedalusVMWithExternals.cpp
  scripting/ScriptSymbols.hpp
  scripting/ScriptSymbols.cpp
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
  scripting/ScriptVMInterface.hpp
  scripting/ScriptVMInterface.cpp
  scripting/ScriptObjectMapping.hpp
  scripting/ScriptObjectMapping.cpp
  original-content/VirtualFileSystem.hpp
  original-content/VirtualFileSystem.cpp
  original-content/OriginalGameFiles.hpp
  original-content/OriginalGameFiles.cpp
  engine-content/EngineContent.cpp
  engine-content/EngineContent.hpp
  engine-content/internal/FindEngineContent.cpp
  engine-content/internal/FindEngineContent.hpp
  world/internals/ConstructFromZEN.hpp
  world/internals/ConstructFromZEN.cpp
  world/GameWorld.hpp
  world/GameWorld.cpp
  gui/skin_gothic.hpp
  gui/skin_gothic.cpp
  world/internals/ImportSingleVob.hpp
  world/internals/ImportSingleVob.cpp
  )

target_link_libraries(REGothEngine PUBLIC bsf BsZenLib)
target_include_directories(REGothEngine PUBLIC .)

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
