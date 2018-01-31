mkdir build-ar
cd build-ar
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=release -DARM=1 -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j8 ../source
