[![Built with Spacemacs](https://cdn.rawgit.com/syl20bnr/spacemacs/442d025779da2f62fc86c2082703697714db6514/assets/spacemacs-badge.svg)](http://spacemacs.org)
[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/REGoth/Lobby)

# REGoth-bs 

OpenSource-Reimplementation of the zEngine, used by the game "Gothic" and "Gothic II".

We have an active Chatroom on [Gitter](https://gitter.im/REGoth/Lobby)! If you want to reach us, this is the best way!

# Documentation

https://regoth-project.github.io/REGoth-bs/index.html

# Building

## Engine

The project is built via CMake. You will need at least CMake 3.9.0.

### Linux

To build the project for Linux, 
run the following commands from within the directory where you cloned REGoth:

```sh
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cmake --build . --parallel 8
```

**Note**: If the linker complains about missing `.so` files when you try to run an executable,
try running it as `LD_LIBRARY_PATH=../lib ./REGothSomething`.

### Windows using Visual Studio Code

This is the recommended way for developing on Windows. Visual Studio Code works
really well with the CMake-files of REGoth. Just make sure to install the
official C/C++ and CMake extensions.

### Windows using Visual Studio

To build the project for Windows, you need to be aware of some common pitfalls:

 1. Always build x64, as bs:f does not support x86. To do that, make sure to use
    the correct Visual-Studio-Generator.
 2. Similar to `make`, Visual Studio will not do parallel builds by default.

These are the commands:

```sh
md build
cd build
cmake -G"Visual Studio 15 2017 Win64" ..
cmake --build . --config RelWithDebInfo --parallel 8
```

## Building the Documentation

You will need:

 * Doxygen
 * Python
  * Sphinx (install via `pip install sphinx`)
  * Breathe (install via `pip install breathe`)
  * Sphinx ReadTheDocs-Theme (install via `pip install sphinx_rtd_theme`)

With a cmake-project generated, run this from within the `build`-directory:

```sh
cmake --build . --target REGoth_docs
```

This will generate the documentation as HTML into `build/docs-source/html`. 
To update Github-pages, copy the contents of that directory into the `docs` directory at the repository root.
