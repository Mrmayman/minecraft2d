# minecraft2d
## About
A 2D Minecraft clone in C++ and SDL2, which also supports controllers!

## OS Support
minecraft2D officially supports Windows and Linux, but you can run the Windows version in wine for an unsupported platform.
The Windows version is tested on both Windows 11 and Wine.
The Linux version is tested on Linux Mint Cinnamon.

I will not make a native macOS version because:
- I don't have a Mac and can't afford one
- The Windows version should work perfectly fine in Wine

## Build
To build project:
- Run `./compile.sh` for Linux build.
- Run `./debuglinuxcompile.sh` for Linux build with debug symbols (for use with gdb for example)
- Run `./wincompile.sh` for Windows build.
You will find the linux compiled binary in `./bin/a.out` and the windows compiled binary in `./bin/game.exe`
The required dlls for windows version are bundled with the source code for easier setup.

Don't judge me for my terrible skills. I'm a beginner.
