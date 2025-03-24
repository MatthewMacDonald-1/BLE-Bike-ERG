cd build_mac
cmake --build . --config Release
cp -r ./../src/resources/ ./../bin/resources/
mkdir -p ./../bin/saves/
