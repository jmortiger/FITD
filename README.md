# FITD
Free in the Dark, a Alone in the Dark engine reimplementation.

Long overdue source code update from the version that was released on source forge in the 2000s.
Only buildable with Visual Studio 2019 for now (cmake via build/vs2019.bat) but other platform should be fairly straight forward.

Rendering is being rewritten to bgfx, for better portability. There are still some graphical artifacts.

# Info
This fork is an attempt to fix some minor problems and clarify some things about this specific repo, and hopefully make some pull requests if I ever make any sort of drive to pursue this.
Not many people were exactly knowledgeable on how to compile it, especially since it would return errors since it wouldn't download the dependencies before unless you had an SSH key tied to your Github account.

My main goal is to poke around and make it a bit neater, and potentially provide some insight to FITD itself.
For example, did you know the both the GOG & Steam versions of AITD have an ancient version of FITD in the files? Don't believe me? See for yourself.

**Beware, if there are any crashes, I will do my best to try to find a way to fix them, but don't count on it. This project isn't the only thing I'm working on.**

# Compiling

I've done my best to make some minor changes to the .gitmodules page so that way the project can properly clone itself. It's frustrating, but I think it's better that it's done this way rather than the way yaz0r listed before.

### 1. Clone the repo

```
git clone https://github.com/jmortiger/FITD.git --recurse-submodules
```

it is ***VERY IMPORTANT*** that you add the last argument to the commit, otherwise building the project will give you errors regardless of what you do. I don't know why Git doesn't do this automatically, but you have to clone it like this.

### 2. Use CMake

Using the GUI, you should be able to build the repo you just cloned. Point the source code directory to the new folder, and then set the build directory to wherever you desire.

Click Config. It may come up with a list of configurables, which you can do whatever you wish, but it's best to just leave it at its default for now. (Maybe disable the 3DNow! extension if your CPU doesn't support it?)

After that, click generate.

### 3. Build

After CMake created the folder where the build files are, run the .sln with Visual Studio and build it like you'd do any other project. Wait for a while, and eventually you should have a ready-to-use FITD install.

### 4. Place the .EXE alongside the game

You MUST place the EXE with the game files. If you do not, you will be hit with a plethora of errors.

After that, you should be done.

## Misc. Notes
* I'm using Linux, so tell me if there's any problems on other platforms.
