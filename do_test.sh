#!/bin/zsh

test_lib_include='./test/include';
projext_include='./include';

cpp_version=c++20;

rm ./build/tests;

/usr/bin/clang++ -std=${cpp_version} -I${test_lib_include} -I${projext_include} ./lib/**/*.cpp ./test/lib/*.cpp ./test/tests/*.cpp ./test/main.cpp -o ./build/tests;

./build/tests
