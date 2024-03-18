cpp_version=c++17;
shared_headers=./exercises/headers
exercise_headers=./exercises/Chapter_5/5_2/headers

c++ -std=${cpp_version} -I${shared_headers} -I${exercise_headers} \
  ./exercises/lib/*.cpp \
  ./exercises/Chapter_5/5_2/lib/*.cpp \
  ./exercises/Chapter_5/5_2/main.cpp;

echo "- + 38 -14. +894.0 23.92 .43 -.89 +.24 0.093 +" | ./a.out

rm ./a.out
