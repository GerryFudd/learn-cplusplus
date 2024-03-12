#!/bin/zsh

test_lib_include='./test/include';
project_include='./include';

cpp_version=c++20;

rm ./build/tests;

/usr/bin/clang++ -std=${cpp_version} -I${test_lib_include} -I${project_include} ./lib/**/*.cpp ./test/lib/*.cpp ./test/tests/*.cpp ./test/main.cpp -o ./build/tests;

./build/tests
