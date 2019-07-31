#pragma once

#include <iostream>
#include <memory>

#include <FileSystem/BsPath.h>

namespace REGoth
{
  /**
   * Parses the given command line arguments in `argv` and returns a newly constructed `EngineConfig`
   * of subtype `T`.
   *
   * The returned `EngineConfig` can be used to be `std::move`d into an engine (`AbstractEngine`)
   * while construction.
   *
   * @param argc Main's `argc` (argument count).
   * @param argv Main's `argv` (arguments vector).
   * @return Unique pointer to a newly constructed and valid `EngineConfig` of subtype `T`.
   *
   * @note A static assertion ensures that `T` is an `EngineConfig`.
   * @ingroup core
   * @headerfile core/ParseArguments.hpp
   */
  template <class T>
  std::unique_ptr<const T> parseArguments(int argc, char** argv);
}  // namespace REGoth

// Template implementations.
#include <core/ParseArguments.tpp>
