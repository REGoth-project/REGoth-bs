#pragma once

#include <iostream>
#include <memory>

#include <FileSystem/BsPath.h>

/**
 * Allows using the `bs::Path` data type together with `cxxopts`.
 *
 * @param str Input stringstream.
 * @param path Path to write data to.
 * @return The original stringstream.
 */
std::stringstream& operator>>(std::stringstream& str, bs::Path& path);

namespace REGoth
{
  class AbstractEngine;

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
   * @headerfile core/functions.hpp
   */
  template <class T>
  std::unique_ptr<const T> parseArguments(int argc, char** argv);

  /**
   * Bootstrap and run the given engine.
   *
   * This is a convenience function to properly setup, bootstrap, and operate an engine.
   *
   * @param engine Concrete engine to run.
   * @return Windows errorlevel / POSIX status code.
   *
   * @ingroup core
   * @headerfile core/functions.hpp
   */
  int runEngine(AbstractEngine& engine);
}  // namespace REGoth

// Template implementations.
#include <core/functions.tpp>
