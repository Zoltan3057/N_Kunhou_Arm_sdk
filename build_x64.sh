mkdir build-r
cd build-r
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=release -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j8 ../source
