# MedFlix

**Me**: Mom, I want Netflix.

**Mom**: We have Netflix at home.

Netflix at home:

![MedFlix screenshot of Movie Info page for Jaws](https://github.com/yosoyducc/medflix/blob/main/medflix.webp?raw=true)

## What???

MedFlix is a program for movies management. This is our team's submission
for Lab 13. It is written for Med Mogasemi's CS 1D class at Saddleback.

Here's a list of the [awesome features](#the-sidebar) it has.

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

## How To Use

MedFlix requires registration to use the program features.

### First time use

On first launch, you'll need to register. To change login mode, click the
dropdown on the upper right hand corner of the Account Manager window.
Select "REGISTER", enter your credentials, and press "Register me!" Make
sure to remember your password, because it's nearly impossible to recover!

From there, switch back to "SIGN IN" mode and just hit "Sign me in!" You'll
be immediately redirected to the "Home" tab where recommendations appear.

### The Sidebar

When logged in, you have access to six tabs:

1.  **Home**: A page with ~50 recommended movies. Click on any movie in the
    list to view more information about it. Requires a manual refresh,
    press the refresh button in the upper right to request new
    recommendations.

2.  **Favorites**: List of favorited movies. Must be manually refreshed when
    movies are added/removed from the list.

3.  **Search (Filmhunter)**: Enables searching the movies database.

4.  **Movie Info**: Displays information about a particular movie. Allows user
    to rate the current movie, add to favorites, and mark as watched.

5.  **Account**: Allows you to sign out (when logged in only).

6.  **Quit**: Prompts the user to exit the program.

### Actions

Rating a movie as "Watched" (the eye) or "Superliked" (the star) gives weight
to movies like that one (movies in the same genre) for the recommendations
system. If you exclusively like movies in Comedy, your recommendations will be
over- saturated with Comedy, with some other genres thrown in for good measure.
You can also mark a movie as Liked (1UP) or Disliked (skull), but these do
nothing right now.

Adding a movie to Favorites (the heart) will make it appear in the Favorites
tab on the next refresh of the tab.

### Keyboard shortcuts

Program controls are mostly mouse based, but you can navigate the tabs by
pressing CTRL + [tab key] (reference the tabs). For example, Home \[^A\]
can be navigated to by pressing CTRL + A. Some screens, like Account and
Movie Info, allow using Tab / Enter to use the text fields.

## Authors

Copyright (C) 2024 MedFlix authors. All Rights Reserved.
This program is licensed under PolyForm Strict License 1.0.0, a copy of the
license text may be found in the [LICENSE.md](./LICENSE.md) file.

MedFlix is programmed by:
* Aiden Asar ([@AidenAsar](https://github.com/AidenAsar))
* Filip Vracevic ([@yosoyducc](https://github.com/yosoyducc))

Movie posters in poster directory are low-resolution and are used only for
purposes of identifying the movies. This should be fair use under U.S. law.
