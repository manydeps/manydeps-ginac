# manydeps-ginac

[![CMake on Multiple Platforms](https://github.com/manydeps/manydeps-ginac/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/manydeps/manydeps-ginac/actions/workflows/cmake-multi-platform.yml)

[![Bazel on multiple platforms](https://github.com/manydeps/manydeps-ginac/actions/workflows/bazel-multi-platform.yml/badge.svg)](https://github.com/manydeps/manydeps-ginac/actions/workflows/bazel-multi-platform.yml)

![C++17](https://img.shields.io/badge/std-c%2B%2B17-blue) [![C++20_/permissive](https://img.shields.io/badge/std-c%2B%2B20_%2Fpermissive-blue)](https://devblogs.microsoft.com/cppblog/msvc-cpp20-and-the-std-cpp20-switch/)

***Beware that c++20 only works with /permissive flag on windows, otherwise we get link errors LNK2019***.

This is a demonstration project from the [ManyDeps](https://github.com/manydeps),
for the C/C++ [GINAC library](https://www.ginac.de/), using [CLN library from GINAC](https://www.ginac.de/CLN/) with GMP library using package managers (vcpkg and conan) on windows/linux.

This works fine on Windows Visual Studio 2022 and also Linux (including Windows WSL).
For Linux, the classic `gmp` library is used, but for Windows, the alternative `mpir`
is used (since `gmp` currently fails to build on Windows in this repo).

Basic setup in both platforms include: CMake and Ninja.
Also it is necessary to have vcpkg and conan, for the following scripts to work.

- If you want to see only GMP+CLN, check [manydeps-cln](https://github.com/manydeps/manydeps-cln)
- If you want to see only GMP, check [manydeps-gmp](https://github.com/manydeps/manydeps-gmp)

## Running the example on vcpkg

Please follow the next steps carefully.

### First step: get submodule dependencies

vcpkg is installed as shown in [vcpkg.io](vcpkg.io website), as a git submodule.

To get submodules, please run the following commands:

```
git submodule update --init --recursive
git pull --recurse-submodules
```

Check the folder [tools/vcpkg](tools/vcpkg) to see if vcpkg is present.

### Second step: get vcpkg dependencies

Automatically done on Visual Studio 2022! 

## Windows working example

Using system vcpkg:

```
PS C:\Users\XXXX\Source\Repos\manydeps-cln> vcpkg --version
versão do programa de gerenciamento de pacotes vcpkg 2023-06-22-f19f3d9939100085fe2f7e41502d60b08e2083b6
```

Using local vcpkg (from submodule):

```
vcpkg package management program version 2023-08-09-9990a4c9026811a312cb2af78bf77f3d9d288416
```

## Linux working example

Using system vcpkg (from devcontainer image):

```
$ vcpkg --version
vcpkg package management program version 2023-01-24-8a88d63f241d391772fbde69af9cab96c3c64c75
```

Using local vcpkg (from submodule):

```
vcpkg package management program version 2023-08-09-9990a4c9026811a312cb2af78bf77f3d9d288416
```

<!--

#### On Windows

On Windows Visual Studio, open the **Developer Command Prompt** and execute:

```
.\tools\vcpkg\bootstrap-vcpkg.bat
.\script-deps.bat
```

#### On Linux (or WSL)

On Linux (or WSL), just run:

```
./tools/vcpkg/bootstrap-vcpkg.sh 
./script-deps.sh
```

-->

## Common errors

On Linux, sometimes dependencies are not fully available, so please double check:

### Latest CMake on Linux

```
python3 -m pip install --upgrade
```

### Ninja on Linux

```
apt-get install ninja-build
```

### General build dependencies on Linux

vcpkg requires some packages, such as pkg-config and autoconf:

```
apt-get install autoconf automake libtool pkg-config
```

## Using CMakePresets for IDE

Update your CMakePresets.json to include the desired toolchain.

### vcpkg toolchain

```{.json}
"toolchainFile": "${sourceDir}/deps/vcpkg/scripts/buildsystems/vcpkg.cmake",
```

Read more on vcpkg integration: https://learn.microsoft.com/pt-br/vcpkg/users/buildsystems/cmake-integration

### conan toolchain

Install conan with:

```
python3 -m pip install conan
```

Setup toolchain with cmake and conan:

```{.json}
"cacheVariables": {
    "CMAKE_BUILD_TYPE": "Release",
    "CMAKE_TOOLCHAIN_FILE": "build-conan/conan_toolchain.cmake"
},
```

## Discussion and common errors

The vcpkg tries to create some CMake toolchain with PkgConfig, 
in order to get dependencies.
We try to build them as **STATIC** libraries: 
`libgmp.a` (on linux) and `gmp.lib` (on windows).

This is the expected configuration after `vcpkg install`:

```
[cmake]     #  gmp
[cmake]     find_package(PkgConfig REQUIRED)
[cmake]     pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)
[cmake]     target_link_libraries(main PkgConfig::gmp)
[cmake] 
[cmake]     # gmpxx
[cmake]     find_package(PkgConfig REQUIRED)
[cmake]     pkg_check_modules(gmpxx REQUIRED IMPORTED_TARGET gmpxx)
[cmake]     target_link_libraries(main PkgConfig::gmpxx)
```

However, this failed in both Linux and Windows (in our experiments here),
 so we need to make some adjustments (**A LOT OF ADJUSTMENTS FOR WINDOWS**).

### Trying to understand the errors

The idea is to load packages locally on `build/vcpkg_installed/` folder 
(generated by vcpkg on our `script-deps`).

We still could not fully understand why it fails so bad in Windows,
but it seems that some files such as `gmp.pc` are missing!
For Linux they exist on `build/vcpkg_installed/x64-linux/lib/pkgconfig/` folder.

If you find a better solution, please Let Us Know!

## Known Issues

For CLN, the cmake build cannot be made using "-B build", 
since it only works changing directory to build folder (pattern "cd build && cmake ..").
The failure happens with "AsmUnderscode.cmake" test, and some attempts to solve this issue
have currently failed.

### Fix on `factor.cpp` file

We cannot use C++20 due to linking errors in windows, so we take C++17.
However, file `factor.cpp` uses designated initializers from c++20, and need to fix that.
Please apply that patch in tools/patch before building!

## License

[GPLv3+ License](https://www.gnu.org/licenses/gpl-3.0.html)

License is a combination of:

- GiNaC (GPL-2.0-or-later)
- CLN (GPL-2.0-or-later)
- GMP (GPL-2.0-or-later OR LGPL-3.0-or-later)
