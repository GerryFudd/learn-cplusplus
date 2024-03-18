cpp_version=c++17;
shared_headers=./exercises/headers
exercise_headers=./exercises/Chapter_5/5_1/headers

c++ -std=${cpp_version} -I${shared_headers} -I${exercise_headers} \
  ./exercises/lib/*.cpp \
  ./exercises/Chapter_5/5_1/lib/*.cpp \
  ./exercises/Chapter_5/5_1/main.cpp;

echo "- + 38 -14 +894 +" | ./a.out

rm ./a.out
