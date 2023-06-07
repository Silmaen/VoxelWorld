# Owl

![](assets/logo/logo_owl.png)

Owl aims to be a simple game engine. The main goal of this engine is learning game engine
development.

## Platform

We try to be multiplatform :

* Windows x64 mingw (g++ or clang++)
* Linux x64 (g++ or clang++)
* Linux Arm64

These platform requires a graphic card that support OpenGL 4.5.

## Dependencies

Dependencies are managed by my dependency tool [DepManger](https://github.com/Silmaen/DepManager).

All the dependencies and their recipes for DepManager are available in a separate
repository [here](https://github.com/Silmaen/OwlDependencies).
Most of then are configured as git submodules.

## RoadMap

* Misc
    * [ ] Refactor unclean code
    * [ ] Refactor documentation
    * [ ] Add unit testing
* Renderder
    * [ ] Add a legacy OpenGL renderer (version < 4.5 )
        * [ ] Make it work on Old GraphicCard on x64
        * [ ] Make it Work on Raspberry Pi 3

