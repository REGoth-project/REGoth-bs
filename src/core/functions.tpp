#include <core/functions.hpp>

#include <memory>
#include <string>

#include <Allocators/BsStackAlloc.h>
#include <FileSystem/BsPath.h>

#include <cxxopts.hpp>

#include <core/EngineConfig.hpp>

template <class T>
std::unique_ptr<const T> REGoth::parseArguments(int argc, char** argv)
{
  static_assert(std::is_base_of<EngineConfig, T>(),
                "Template class must have `REGoth::EngineConfig` as base class");

  std::unique_ptr<T> config = std::make_unique<T>();

  // Some utility functions involving bs::Path fail if this was not called prior.
  bs::MemStack::beginThread();

  bool help;
  bool version;

  cxxopts::Options options{argv[0], "REGoth - zEngine Reimplementation."};

  // Add general options.
  const std::string grp = "General";
  options.add_option(grp, "h", "help", "Print this help message", cxxopts::value<bool>(help), "");
  options.add_option(grp, "", "version", "Print the REGoth version", cxxopts::value<bool>(version),
                     "");
  options.add_option(grp, "v", "", "Increase verbosity level (counts v's)", cxxopts::value<bool>(),
                     "");

  // Add options (engine options and specialised ones).
  config->registerCLIEngineOptions(options);
  config->registerCLIOptions(options);

  // Parse argv.
  cxxopts::ParseResult result = options.parse(argc, argv);

  // Print help if `-h` or `--help` is passed and exit.
  if (help)
  {
    std::cout << options.help() << std::endl;
    std::exit(EXIT_SUCCESS);
  }

  // Print REGoth version if `--version` is passed and exit.
  if (version)
  {
    std::cout << "Not yet implemented" << std::endl;
    std::exit(EXIT_SUCCESS);
  }

  // Set verbosity level.
  config->verbosity = static_cast<unsigned int>(result.count("v"));

  // Game executable path must be set manually here.
  config->engineExecutablePath = bs::Path{argv[0]};

  // Verify configuration.
  config->verifyCLIEngineOptions();
  config->verifyCLIOptions();

  // Clean up.
  bs::MemStack::endThread();

  return config;
}
