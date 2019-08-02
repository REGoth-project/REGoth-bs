.. _execs:

Executables
===========

REGoth features several executables, whose usage is documented here.


General
-------

All executables have the following command line options available:

- ``-h``, ``--help``: Print the help message
- ``--version``: Print the version of REGoth
- ``-v``: Increase the verbosity level


Core engine
-----------

The core engine's command line options are the following:

- ``-g``, ``--game-assets``, or the first positional argument: Path to a valid Gothic or Gothic II
  installation
- ``--video-x-res``: X resolution
- ``--video-y-res``: Y resolution
- ``--video-fullscreen``: Whether to run the game in fullscreen


REGothWorldViewer
-----------------

This custom engine allows viewing arbitrary worlds.  The world to load can be configured using
the ``-w`` or ``--world`` command line arguments.

Available worlds for Gothic are:

- "world"
- "oldmine"
- "freemine"
- "orcgraveyard"
- "orctempel"

Available worlds for Gothic II are:

- "newworld" (Island of Khorinis)
- "oldworld" (Valley of Mines)
- "addonworld" (Jharkendar - Only available with the addon Gothic II: Night of the Raven)
- "dragonisland" (Island of Irdorath)
