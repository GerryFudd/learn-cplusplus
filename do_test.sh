#!/bin/zsh

boost_include='/usr/local/bin/boost/include';
projext_include='./include';

cpp_version=c++20;

rm ./build/tests;

/usr/bin/clang++ -std=${cpp_version} -I${boost_include} -I${projext_include} ./lib/**/*.cpp ./test/*.cpp -o ./build/tests;

./build/tests
