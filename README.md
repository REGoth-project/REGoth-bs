[![Built with Spacemacs](https://cdn.rawgit.com/syl20bnr/spacemacs/442d025779da2f62fc86c2082703697714db6514/assets/spacemacs-badge.svg)](http://spacemacs.org)
[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/REGoth/Lobby)


<p align="center">
  <img src="https://user-images.githubusercontent.com/11406580/58186975-2cdbb880-7cb6-11e9-9a6d-b38326bc3edb.png" />
  <p align="center"><em>OpenSource-Reimplementation of the zEngine, used by the game "Gothic" and "Gothic II"</em></p>
</p>

---

<p align="center">
  <img src="https://www.mobygames.com/images/promo/l/30647-gothic-screenshot.jpg" />
  <p align="center"><em>(Official Gothic promotional Image)</em></p>
</p>



# Developer Chat-Room

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

**Note**: If you get complains about missing `.so` files when you try to run an executable,
try running it as `LD_LIBRARY_PATH=../lib ./REGothSomething`. Also, you might get an error indicating `libPhysX3_x64.so` not being found. You need to go into `REGoth-bs/lib/bsf/Dependencies/PhysX/lib/` and copy all `.so`-files to the location where `libbsf.so` was built, usually `REGoth/build/lib`.

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

### Windows using Ninja

The Ninja-Build System performs faster than MsBuild since it is able to run more tasks in parallel: While MsBuild can only build different projects in parallel, Ninja acts as you would expect and is able to build multiple files in parallel, even if they are from within the same project.

Unfortunately, using it on Windows is a little bit tricky. From a *x64 Native Tools Command Prompt*, navigate to where you cloned REGoth and run:

```sh
md build
cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER="cl.exe" -DCMAKE_CXX_COMPILER="cl.exe" -DMSVC_TOOLSET_VERSION=140 ..
ninja
```

Note that Visual Studio Code seems to figure this out automatically and uses Ninja by default.

## Using our own copy of Vcpkg

REGoth uses [Vcpkg](https://github.com/microsoft/vcpkg) internally. To make building as seamless as possible, the build script will take care of getting a copy of Vcpkg, bootstrap it and install the required packages. All this happens inside the `build`-directory so it doesn't clutter your system with stuff you don't want.

If you want to use your own copy of Vcpkg, just pass the toolchain-file as usual.


## Building without Vcpkg

If you don't want to have anything to do with Vcpkg, you can also choose to supply the required packages yourself. To do this, you need to pass `-DSKIP_AUTOMATE_VCPKG=ON` to CMake. Now you're completely on your own to gather the required dependencies for REGoth.


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


# Special Thanks

 - @truepaddii for creating REGoth's logo!
