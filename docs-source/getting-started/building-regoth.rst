.. _building_regoth:

Building REGoth
===============

This page documents how to build REGoth on Linux or Windows.  The project is built via CMake.  For
all platforms, CMake version 3.13 is required.

Linux
-----


System packages
~~~~~~~~~~~~~~~

For **Debian** (Buster or newer), **Ubuntu** (bionic or newer), or derivatives, install the
following packages:

.. code-block:: sh

  # Required tooling packages.
  $ sudo apt install git cmake make gcc

  # Required dependencies.
  $ sudo apt install libx11-dev libxcursor-dev libxi-dev libicu-dev libgl1-mesa-dev \
  libglu1-mesa-dev freeglut3-dev libphysfs-dev libsquish-dev uuid-dev

  # Development dependencies (optional).
  $ sudo apt install doxygen plantuml

For **Fedora**, install the following packages:

.. code-block:: sh

  # Required tooling packages.
  $ sudo dnf install git cmake make gcc

  # Required dependencies.
  $ sudo dnf install libX11-devel libXcursor-devel libXi-devel libicu-devel mesa-libGL-devel \
  mesa-libGLU freeglut-devel physfs-devel libsquish-devel libXrandr-devel libuuid-devel

  # Development dependencies (optional).
  $ sudo dnf install doxygen plantuml

For **Arch Linux**, **Manjaro**, or derivatives, install the following packages:

.. code-block:: sh

  # Required tooling packages.
  $ sudo pacman -S git cmake make gcc

  # Required dependencies.
  $ sudo pacman -S physfs libx11 libxcursor libxi icu mesa freeglut libxrandr

  # Development dependencies (optional).
  $ sudo pacman -S doxygen plantuml

.. note:: If a required package is missing, or you managed to compile REGoth on a distribution not
  explicitly listed above, we would *greatly* appreciate a pull request or GitHub issue with this
  information.


Building
~~~~~~~~

First we need to fetch the source code and prepare our build environment:

.. code-block:: sh

  # First, clone the Git repository together with its submodules.
  $ git clone --recursive https://github.com/REGoth-project/REGoth-bs.git
  $ cd REGoth-bs

  # Prepare the build directory.
  $ mkdir build
  $ cd build

  # Run CMake.
  $ cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..

Now we can build the project (if you want to use less cores than available, replace ``$(nproc)``
with the desired number.  You might want to do this if you have little RAM, expect up to 2GB of RAM
usage per core):

.. code-block:: sh

  # Compile REGoth.
  $ cmake --build . --parallel $(nproc)


Windows
-------


Using Visual Studio Code
~~~~~~~~~~~~~~~~~~~~~~~~

This is the recommended way for developing on Windows.  Visual Studio Code works really well with
the CMake files of REGoth.  Just make sure to install the official C/C++ and CMake extensions.


Using Visual Studio
~~~~~~~~~~~~~~~~~~~

To build the project for Windows, you need to be aware of some common pitfalls:

 1. Always build x64, as bs::framework does not support x86. To do that, make sure to use the
    correct Visual-Studio-Generator.
 2. Similar to ``make``, Visual Studio will not do parallel builds by default.

These are the commands:

.. code-block:: sh

  > md build
  > cd build
  > cmake -G"Visual Studio 15 2017 Win64" ..
  > cmake --build . --config RelWithDebInfo --parallel 8


Using Ninja
~~~~~~~~~~~

The Ninja build system performs faster than MsBuild since it is able to run more tasks in parallel:
While MsBuild can only build different projects in parallel, Ninja acts as you would expect and is
able to build multiple files in parallel, even if they are from within the same project.

Unfortunately, using it on Windows is a little bit tricky.  From a
*x64 Native Tools Command Prompt*, navigate to where you cloned REGoth and run:

.. code-block:: sh

  > md build
  > cd build
  > cmake -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER="cl.exe" -DCMAKE_CXX_COMPILER="cl.exe" -DMSVC_TOOLSET_VERSION=140 ..
  > ninja

Note that Visual Studio Code seems to figure this out automatically and uses Ninja by default.
