# World-Engines

A bluetooth enabled bike exercise program.


# Build 

Create the build directory.

```
mkdir build
cd build
cmake ..
```

Build the project (must be in build directory) and copy over the resources folder to the bin folder.

```
cmake --build . --config Release
cp -r ./../src/resources/ ./../bin/resources/
mkdir -p ./../bin/saves/
```
