# MedFlix

**Me**: Mom, I want Netflix.

**Mom**: We have Netflix at home.

Netflix at home:

## What???

MedFlix is a program for movies management. This is our team's submission
for Lab 13. It is written for Med Mogasemi's CS 1D class at Saddleback.

Here's just some of the awesome features:

* An exit text box prompt (lol)

Please see the [assignment details](./ASSIGNMENT.md) for information.

## Build instructions

### Requirements

Everything AND the bagel, unfortunately.

You'll need:

* [raylib 5.5](https://github.com/raysan5/raylib/releases/tag/5.5)
* [raygui 4.0](https://github.com/raysan5/raygui/releases/tag/4.0)

For raylib, download the prebuilt archive for your platform. Alternatively, if
you're a masochist, you can compile it by yourself from scratch.

For raygui, download the source code (either zip or tar.gz works). From the
src/ directory, copy the raygui.h header file to this directory.

### Steps

> Derived from the [raylib wiki](https://github.com/raysan5/raylib/wiki).

Varies from platform to platform. In general, you'll want to compile all *.cpp
files in this directory and link against raylib. On Windows with MSVC compiler,
I imagine the build command would look something like this:

    cl /EHsc /Fe:MedFlix.exe *.cpp gdi32.lib msvcrt.lib raylib.lib winmm.lib /Ic:\path\to\raylib\include /link /libpath:c:\path\to\raylib\lib /NODEFAULTLIB:libcmt

On Linux, this command works with the GNU C++ compiler:

    g++ -o MedFlix.out *.cpp /path/to/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11

## Usage

Escape (or hit X on the window bar) to exit.

## Authors

Copyright (C) 2024 MedFlix authors. All Rights Reserved.

* Aiden Asar
* Filip Vracevic
