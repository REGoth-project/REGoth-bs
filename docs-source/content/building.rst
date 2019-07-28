Building from Source
====================


Make sure to do a *recursive* clone::

   git clone --recursive https://github.com/REGoth-project/REGoth-bs.git


If you have already cloned the repository, run this in the project root folder::

   git submodule update --init --recursive


The project is built via CMake. You will need at least CMake 3.13.0.

Linux
-----

For **Debian** and its derivatives, install the following packages::

   sudo apt install libx11-dev libxcursor-dev libxi-dev \
                    libicu-dev libgl1-mesa-dev libglu1-mesa-dev \
                    freeglut3-dev libphysfs-dev libsquish-dev

For **Fedora** install the following packages::

   sudo dnf install libX11-devel libXcursor-devel \
                    libXi-devel libicu-devel mesa-libGL-devel \
                    mesa-libGLU freeglut-devel physfs-devel \
                    libsquish-devel libXrandr-devel

For other distributions, just look up the corresponding package names for these
libraries (and please make a PR complementing these information).

To build the project for Linux, run the following commands from within the
directory where you cloned REGoth::

   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
   cmake --build . --parallel $(nproc)

The binaries are then put into ``build/bin``.
 
Windows using Visual Studio Code
--------------------------------

This is the recommended way for developing on Windows. Visual Studio Code works
really well with the CMake-files of REGoth. Just make sure to install the
official C/C++ and CMake extensions.

Windows using Visual Studio
---------------------------

To build the project for Windows, you need to be aware of some common pitfalls:

1. Always build x64, as bs:f does not support x86. To do that, make sure to use
   the correct Visual-Studio-Generator.
2. Similar to ``make``, Visual Studio will not do parallel builds by default.

These are the commands::

   md build
   cd build
   cmake -G"Visual Studio 15 2017 Win64" ..
   cmake --build . --config RelWithDebInfo --parallel 8

Windows using Ninja
-------------------

The Ninja-Build System performs faster than MsBuild since it is able to run more
tasks in parallel: While MsBuild can only build different projects in parallel,
Ninja acts as you would expect and is able to build multiple files in parallel,
even if they are from within the same project.

Unfortunately, using it on Windows is a little bit tricky. From a *x64 Native
Tools Command Prompt*, navigate to where you cloned REGoth and run::

   md build
   cd build
   cmake -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER="cl.exe" -DCMAKE_CXX_COMPILER="cl.exe" -DMSVC_TOOLSET_VERSION=140 ..
   ninja

Note that Visual Studio Code seems to figure this out automatically and uses Ninja by default.

Dependencies via Vcpkg
----------------------

REGoth uses Vcpkg to resolve some third-party dependencies. On configuration time, CMake will
download a copy of Vcpkg for internal use. If you don't want that or want to use the system installed
Vcpkg, you can configure that.

Using our own copy of Vcpkg
^^^^^^^^^^^^^^^^^^^^^^^^^^^

REGoth uses Vcpkg_ internally. To make building as seamless as possible, the
build script will take care of getting a copy of Vcpkg, bootstrap it and install
the required packages. All this happens inside the ``build``-directory so it
doesn't clutter your system with stuff you don't want.

If you want to use your own copy of Vcpkg, just pass the toolchain-file as usual.

.. _Vcpkg: https://github.com/microsoft/vcpkg

Building without Vcpkg
^^^^^^^^^^^^^^^^^^^^^^

If you don't want to have anything to do with Vcpkg, you can also choose to
supply the required packages yourself. To do this, you need to pass
``-DSKIP_AUTOMATE_VCPKG=ON`` to CMake. Now you're completely on your own to gather
the required dependencies for REGoth.


Building the Documentation
--------------------------

You will need:

* Doxygen
* Python
* Sphinx (install via ``pip install sphinx``)
* Breathe (install via ``pip install breathe``)
* Sphinx ReadTheDocs-Theme (install via ``pip install sphinx_rtd_theme``)

With a cmake-project generated, run this from within the ``build``-directory::

   cmake --build . --target REGoth_docs

This will generate the documentation as HTML into ``build/docs-source/html``.
To update Github-pages, copy the contents of that directory into the ``docs``
directory at the repository root.

