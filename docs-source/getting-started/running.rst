.. _running_regoth:

Running REGoth
==============

Once you have downloaded a build of REGoth or built it from source, you will find a number of
binaries to play with, like ``REGothWorldViewer``.

The most easy way to set up REGoth correctly is to drop everything somewhere into the Gothic game
installation you want to play.  That means, create a sub-directory anywhere you like and put the
executables and everything that goes with it there.

For example, place it like this::

   > C:/Games/Gothic2/REGoth

Or like this::

   > C:/Games/Gothic2/REGoth/Nightly-build-123/

Or even like this::

   > C:/Games/Gothic2/this/is/some/bogus/path

All of those will work.

.. note::

  REGoth will search the directory tree upwards for a Gothic or Gothic II game installation, you
  can freely choose where you put REGoth, as long as it is somewhere inside a Gothic game directory.


Running from outside of a Gothic installation
---------------------------------------------

If you are building REGoth from source, it may be more comfortable to run it from the build
directory instead of copying it over to a Gothic installation.  All REGoth executables accept a path
to a Gothic game root as first command line argument.

For example, you can run REGoth like this::

  $ ./REGothWorldViewer ~/games/Gothic2

.. note::

  For all executables, you can specify ``./REGothXXX --help`` to get an overview of all available
  options.  You can also see :ref:`here <execs>` for an overview.

.. note::

  **Windows**: When building from source, not all required DLLs are copied into the build directory
  automatically as of now.  If you get complains about ``bsfPhysX.dll`` failing to load, try to copy
  the DLLs inside ``REGoth-bs/lib/bsf/Dependencies/PhysX/lib/`` next to your REGoth executables.

.. note::

  **Linux**: If you get complains about missing ``.so`` files when you try to run an executable, try
  running it as ``LD_LIBRARY_PATH=../lib ./REGothSomething``. Also, you might get an error
  indicating ``libPhysX3_x64.so`` not being found.  You need to go into
  ``REGoth-bs/lib/bsf/Dependencies/PhysX/lib/`` and copy all ``.so``-files to the location where
  ``libbsf.so`` was built, usually ``REGoth/build/lib``.


The asset cache
---------------

When loading a world for the first time, REGoth will convert the resources of the original Gothic
game into a more modern format faster to load next time.  If you want to re-import everything,
simply delete the `cache/` directory next to your REGoth executables.

If you only want to re-import a world, you can delete ``cache/worlds`` while leaving the rest for
faster startup time.
