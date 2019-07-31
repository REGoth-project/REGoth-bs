#pragma once

namespace REGoth
{
  class Engine;

  /**
   * Bootstrap and run the given engine.
   *
   * This is a convenience function to properly setup, bootstrap, and operate an engine.
   *
   * @param engine Concrete engine to run.
   * @return Windows errorlevel / POSIX status code.
   *
   * @ingroup core
   * @headerfile core/RunEngine.hpp
   */
  int runEngine(Engine& engine);
}  // namespace REGoth
