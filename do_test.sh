#!/bin/bash

test_lib_include='./test/include';
project_include='./include';

cpp_version=c++20;

/usr/bin/gcc -std=${cpp_version} -I${test_lib_include} -I${project_include} ./lib/**/*.cpp ./test/lib/*.cpp ./test/tests/*.cpp ./test/main.cpp -lunwind -lstdc++ -o ./build/tests;

./build/tests
