cpp_version=c++17;

c++ -std=${cpp_version} ./exercises/Chapter_1/1_10.cpp;

echo 'This\tline\tis\tseparated\tby\ttabs.\nThis\\line\\is\\separated\\by\\bs.\nThese\b words\b are\b shortened\b by\b one\b.\n' | ./a.out

rm ./a.out
