# World-Engines

A bluetooth enabled bike exercise program.


# Build 

Use cmake and visual studio

OR

### Command line

Create the build directory.

```
mkdir build

cd build

cmake ..
```

Build the project (must be in build directory) and copy over the resources folder to the bin folder.

```
cmake --build .
cp -r ./../src/resources/ ./../bin/resources/
```
