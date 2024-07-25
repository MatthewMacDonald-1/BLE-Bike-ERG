# BLE Bike ERG

A bluetooth low energy enabled bike exercise program.

This program supports reading power, heart rate, and cadence sensor data, and can set the target power for trainers that support the BLE Fitness Training Machine Service (FTMS).

This program also supports writing the the recorded workout data to both csv and fit files.

## Workouts

To add workouts or modify the workout list have a read of [Workouts.md](/Workouts.md)

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
