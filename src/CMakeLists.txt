add_library(samples-common
  common/BsCameraZoomer.cpp
  common/BsFPSCamera.cpp
  common/BsFPSWalker.cpp
  common/BsObjectRotator.cpp
 )

target_include_directories(samples-common PUBLIC common)
target_link_libraries(samples-common PUBLIC bsf)

add_library(REGothEngine
  REGothEngine.cpp
  components/CharacterState.cpp
  components/VisualCharacter.cpp
  components/VisualStaticMesh.cpp
  components/StartSpot.cpp
  components/Spot.cpp
  components/NodeVisuals.cpp
  components/Visual.cpp
  components/VisualMorphMesh.cpp
  animation/StateNaming.cpp
  animation/Animation.cpp
  scripting/daedalus/REGothDaedalusVM.cpp
  scripting/daedalus/DATSymbolStorageLoader.cpp
  scripting/daedalus/DaedalusClassVarResolver.cpp
  scripting/daedalus/DaedalusStack.cpp
  scripting/daedalus/DaedalusDisassembler.cpp
  scripting/daedalus/DaedalusVMWithExternals.cpp
  scripting/ScriptSymbols.cpp
  scripting/ScriptVM.cpp
  scripting/ScriptSymbolQueries.cpp
  scripting/ScriptClassTemplates.cpp
  original-content/VirtualFileSystem.cpp
  world/ConstructFromZEN.cpp
  gui/skin_gothic.cpp
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

add_executable(REGothFontViewer main_FontViewer.cpp)
target_link_libraries(REGothFontViewer REGothEngine samples-common)

add_executable(REGothScriptTester main_ScriptTest.cpp)
target_link_libraries(REGothScriptTester REGothEngine samples-common)
