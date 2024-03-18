cpp_version=c++17;

c++ -std=${cpp_version} ./exercises/Chapter_1/1_13.cpp;

./a.out <<END
This is a chunk of text with words of varrying sizes.
It is a block that spans multiple lines; it even gets rather sesquepedalian.
Some words aren't all letters, some are even far-fetched, which makes this tricky.
There is 1 numerical.
END

# Expected results
# 1: 3, 2: 7, 3: 2, 4: 10, 5: 11, 6: 2, 7: 1, 8: 2, 9: 1, 10: 1, 14: 1

rm ./a.out
