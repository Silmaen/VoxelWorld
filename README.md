# Owl

![GitHub License](https://img.shields.io/github/license/Silmaen/Owl)
![GitHub tag checks state](https://img.shields.io/github/checks-status/Silmaen/Owl/main)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/Silmaen/Owl)
[![GitHub Stars](https://img.shields.io/github/stars/silmaen/Owl)]()

![](assets/logo/logo_owl.png)

Owl aims to be a simple game engine. The main goal of this engine is learning game engine
development.

## Documentation

The full documentation is available on the dedicated website [owl](https:://owl.argawaen.net).

## Platform

We try to be multiplatform and using different compilers:

* Windows x64
    * mingw g++ 13 (and above)
    * mingw clang++ 18 (and above)
* Linux x64 & arm64
    * Ubuntu 24.04 (glibc 2.39) g++ 13 (and above)
    * Ubuntu 24.04 (glibc 2.39) clang++ 18 (and above)

These platform requires a graphic card that supports one of the following renderer:

* `Null` - for no graphic rendering (Server for example)
* `OpenGL` - Use OpenGL 4.5 (you must have Graphics device that supports it)
* `Vulkan` - Use Vulkan 1.3 (you must have Graphics device that supports it)

## Coding Style

The section [coding style](doc/CodingStyle.md) describes the guidelines to follow in the development.

## Dependencies

### Update to latest

To automatically update the dependencies, you can run `python3 -u ci/DependencyCheck.py` that will communicate with the
remote server to get the latest dependencies according to the current platform.

The description of dependencies is located in [depmanager.yml](depmanager.yml).

### Dependencies manager

Dependencies are managed by my dependency tool [DepManger](https://github.com/Silmaen/DepManager).

Dependencies are hosted by my dependency server [DepMangerServer](https://github.com/Silmaen/DepManagerServer).

### Dependencies sources

All the dependencies and their recipes for DepManager are available in a separate
repository [OwlDependencies](https://github.com/Silmaen/OwlDependencies).

Most are configured as git submodules.

## Build

To build the engine Cmake (>3.24) is required.

Most user will use one of the cmake's preset defined.

## RoadMap

* [ ] v1.x.x -- First stable release
* [ ] v0.2.x
    * [ ] OwlDrone Compatible with vulkan
* [ ] v0.1.0 -- First Development Release
