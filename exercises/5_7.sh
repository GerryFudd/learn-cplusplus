g++ -std=c++20 -I./exercises/headers ./exercises/lib/*.cpp ./exercises/Chapter_5/5_7_baseline.cpp -o 5_7_old
g++ -std=c++20 -I./exercises/headers ./exercises/lib/*.cpp ./exercises/Chapter_5/5_7.cpp -o 5_7_new

cat ./exercises/lorem_ipsum.txt | ./5_7_old
cat ./exercises/lorem_ipsum.txt | ./5_7_new

rm ./5_7_old
rm ./5_7_new
