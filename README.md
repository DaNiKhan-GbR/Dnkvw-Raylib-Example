# Dnkvw Raylib Example

A minimal example that demonstrates a possible usage of Dnkvw with Raylib.
May change in time for more elaborate demonstrations.
Breaking changes are expected.

## Building

This project uses CMake. Make sure you have at least version 3.15.
To build it, use the following commands:
```console
dani@khan:dnkvw-raylib-min$ mkdir build
dani@khan:build$ cd build
dani@khan:build$ cmake ..
<CMAKE CONFIGURE OUTPUT HERE>
```

The cmake configuration may take longer because it will automatically download raylib and dnkvw if you don't already have it installed.
If you installed dnkvw in a local directory you can specify the path in the dnkvw_DIR CMake variable.

## Running

If you run it and you don't have dnkvw installed systemwide you may get an error because the shared library (on Window the DLL) of dnkvw is missing.
In that case copy the library (dnvw.\[dll|dylib|so\]) from your dnkvw install directory into the same directory as the executable.
