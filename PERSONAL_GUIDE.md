# Personal guide to c++

Notes for my future self.

## How do you write and run code?

### How do you compile and run?

On macos the command line tool for c++ is called `clang++` and the command `c++` is an alias. In general on a linux system `g++` is the tool to compile c++. You may also use the more generic c compiler, `gcc`, on a linux system if you include the flag `-lstdc++`.

Source files in c++ end with `.cpp`. Here's how you write, compile, and execute such a file.

The entry point of a c++ project is a method called `main`, which returns an int and accepts two arguments. The program terminates in a success state if `main` returns 0 and in an error state otherwise.

> Note
> 
> The first argument is an integer `argc`, which is the argument count. The second argument is an array of character pointers `argv`, which is the arguments vector. Functions in general may be declared as `<return type> <function name>()` regardless of their arguments. This is typically how the `main` method is written when a program is intended to be run with out command line arguments.

The `std::cout` command, which is in the `iostream` library, prints to the console by default. I will use this to write the prototypical "Hello, world!" program. The `std::endl` command from the same library returns the environment-specific newline character, so we will use that as well.

Create a file called `main.cpp` and open it in a text editor. Write the following in that file.

```c++
#include <iostream>

int main() {
  std::cout << "Hello, world!" << std::endl;
  return 0;
}

```

To compile such a file and output the result to an executable run 
```bash
%g++ main.cpp -o output_file
```
to use the specific c++ compiler or
```bash
%gcc main.cpp -lstdc++ -o output_file
```
to use the more general purpose c compiler. *Importantly, anything that uses the -l... flag must come after the list of .cpp files.*

This command will create an executable file at `./output_file`. The flag `-std` tells the compiler to use a specific version of c++ during compilation. The latest version of c++ is 20 (at time of writing, with version 23 in development) so this is the version of c++ we will specify.

Now run `%g++ -std=c++20 main.cpp -o ./hello` and notice that there is a new file called `hello` in your working directory. Execute this file.

```bash
%./hello
Hello, world!
```

### Including code from other files

Every code file is pulled in via the `#include` command. The word "include" is highly significant in the c++ ecosystem. When you write a file that you want to include in another, put this file under the `./include/` directory in your project and tell the compiler about this "include path" with the flag `-I./include`. You should only import header files, which end with `.h` or `.hpp`. These files contain schema definitions for the resources that get imported elsewhere while the implementations live in your `.cpp` files. Let's see how this works if we extract the code that prints "Hello, world!" to a method in another file.

Begin header files with `#ifndef ${resource name}` to avoid declaring your resources multiple times and end them with `#endif` to mark where this if condition ends. Write `#define ${resource name}` just after the `#ifndef` to mark the resource as defined. Put any resource specifications in the body of the file. with the following content. Each block of code may also exist within a namespace to mark which context it is associated with.

Create a header file at `./include/hello.hpp` and give it the following content.

```c++
#ifndef HELLO_FUN
#define HELLO_FUN

namespace hello {
  void say_hello();
}

#endif
```

Create a code file at `./src/hello.cpp` and give it the following content.

```c++
#include <iostream>
#include "hello.hpp"

void hello::say_hello() {
  std::cout << "Hello, world!" << std::endl;
}

```

Modify your `main.cpp` file to look like

```c++
#include "hello.hpp"

int main() {
  hello::say_hello();
  return 0;
}

```

Putting this all together, we will specify `./include` as an include path, `c++20` as the compiler version, `./src/hello.cpp` and `./main.cpp` as source files, and `./import_test` as the resulting executable.

```bash
% g++ -std=c++20 -I./include src/hello.cpp main.cpp -o ./import_test
% ./import_test 
Hello, world!
```

## Setting up your IDE

### VS Code with plugin

VS Code seems to be set up well for c++ development. Download the C/C++ plugin and make sure that your `./includes` directory is set under the `C_Cpp > Default: Include Path` setting. This will tell the intellisense detector where you import your files from so that you can get accurate feedback on missing dependencies and compile errors as you write your code.

### Setting up tests

The standard unit testing library for c++ is Boost.test. [Download the latest version of Boost](https://www.boost.org/users/download/) (I downloaded the `.tar.gz` file when I initially set up the boost library). Unpack the download file and navigate to the resulting directory in your terminal. Then run te bootstrap script followed by the b2 script and specify where you want to keep your boost test dependencies. I put mine into `/usr/local/boost`, which required admin permission.

```zsh
% ./bootstrap.sh
% sudo ./b2 --with-test --prefix=/usr/local/boost install
```

This creates two directories, `/usr/local/boost/include/` and `/usr/local/boost/lib/` that contain all of the relevant header and source files from the boost test library. Now add `/usr/local/boost/include/` to `C_Cpp > Default: Include Path` and you will be able to include boost dependencies.

## Variable declaration

Every variable is assigned a connected block of memory at the time it is declared. For this reason you must declare the type of the variable as it is created. You must even declare the size of compound data types.

All data types in c++ have constructors. So even integers and booleans may be initialized with statements like `int a {-3};` or `bool b {false};`. It is also possible to set a variable equal to an instance of its type `int a = -3;` or `bool b = false;`. When a type has a default constructor and an instance of the class is declared without a constructor call, such as `int c;`, that default constructor is implicitly called.

It is also possible to declare several variables of the same type separated by commas.

```c++
int a {-3}, b = 14, c(-8), d;
```

In this example `a, b, c,` and `d` are `int` type objects, `a == -3`, `b == 14`, `c == -8`, and `d` has an indeterminate value.

## Data structures

### Native

The native data types are available as part of the language's ecosystem. I've broken these into several smaller categories.

#### Primitive CS stuff

`bool` is either `true` or `false` and only takes up one byte. The `byte` data type takes on the values from `-128` to `127`, but doesn't support mathematical operations.

#### Numbers

| class | values | bytes |
| :--- | :----: | ---: |
| `int` | `-(2^(31))` to `2^(31)-1` | 4 |
| `unsigned int` | `0` to `2^(32)-1` | 4 |
| `short` | `-(2^(15))` to `2^(15)-1` | 2 |
| `unsigned short`| `0` to `2^(16)-1` | 2 |
| `long` | `-(2^(63))` to `2^(63)-1` | 8 |
| `unsigned long` | `0` to `2^(64)-1` | 8 |
| `float` | significant digits `-(2^{23})` to `2^(23)-1` | 4 |
|  | times `2^(-128)` to `2^(127)` | |
| `double` | significant digits `-(2^(51))` to `2^(51)-1` | 8 |
|  | times `2^(-1024)` to `2^(1023)` | |


#### Text and junk

`char` represents a single letter or other text symbol. This takes up one byte in memory. `wchar_t` is a character that uses two bytes and represents a symbol from a character set that doesn't fit in a single byte.

## Pointers (introduction to...)

A pointer is a one byte number that points to an address in memory. Pointers are declared as a reference to a specific data type so the variable they point to must be the declared type. An `*` in front of a pointer returns the variable that is stored in the address the pointer points to. A pointer is declared as `${type} * ${name}`. An `&` in front of a variable returns the variable's address in memory. If the variable takes up multiple bytes, this returns the address of the first byte.

```c++
int a = -43;
int * b = &a;
int c = *b;
int * d = &c;
```

In this example the `*b == -43`, `*d == -43`, and `*b == *d`, but `b != d`. Likewise `a == -43`, `c == -43`, and `a == c`, but `&a != &c`. The values of `&a`, `b`, `&c`, and `d` are all addresses in memory that appear as integer-like values that represent the byte's physical order in all of the program's available memory.

Since each pointer is associated with a data type and all of these types have a specific number or bytes assigned to their values, there is a handy way to point to the value of the next instance of that type in memory. For example suppose the characters `char w = 'F', x = 'o', y = 'o', z = 'd', e = '\0;` are stored in adjacent bytes and suppose that `char * word = &w`. Then `*word == 'F'`, `*(word + 1) == 'o'`, `*(word + 2) == 'o'`, `*(word + 3) == 'd'`, and `*(word + 4) == '\0'`. The character `'\0'` is the standard way to mark the end of a string in c.

When a type is assigned multiple bytes, these bytes are always contiguous. For example suppose that `int a{0x3f2e};` and `int b{0xc019};` are stored in adjacent blocks of memory. The variable `a` takes up four bytes and `b` takes up the subsequent four bytes. The pointer `int * c = &a;` knows that it a pointer to objects of type `int`, so `c + 1` points to the address four bytes after `c`. Therefore `*(c + 1) == b == 0xc019`.

## Functions (introduction to...)

### Declaration syntax

A function is declared with a return type and a collection of arguments. For example a function could appear in a header as the following.

```c++
int index_of(char *, char);
```

This function takes a character pointer (a c string) and a character as arguments and returns an integer. The following implements this as a function that returns the first index of the given character in the given string or -1 if the character is missing.

```c++
int index_of(char *a, char b) {
  int i = 0;
  while (*(a + i) != '\0') {
    if (*(a + i) == b) {
      return i;
    }
    i++;
  }
  return -1;
}
```

### Overloading

It is also possible to overload functions with the same name. The following pair of methods will expose a method that behaves identically to the one above but adds another where you can specify the index where you should start searching for the given character.

```c++
int index_of(char *a, char b, unsigned short from) {
  while (*(a + from) != '\0') {
    if (*(a + from) == b) {
      return from;
    }
    from++;
  }
  return -1;
}

int index_of(char *a, char b) {
  return index_of(a, b, 0);
}
```

Note that this function modifies the `from` argument as part of its operations. This will not affect any variables that are passed in because function arguments are copied by default when a function is executed.

### Parameters by reference

When a parameter is specified by its type and no modifiers the function references this argument *by value*. When the function is invoked, a copy of this variable is created with the same type and value. Practically this means two things.

1. The function incurs the processing overhead of copying the variable and the copy is stored in a new location in memory.
1. It is not possible to modify the argument that is passed to the function since it doesn't have a direct reference to the original.

For primitive types such as `int` or `char` the cost of copying the variable is negligible and in most cases it is a feature rather than a bug to avoid mutating the arguments that are passed to your functions.

If the process of copying one of your arguments is unacceptably time consuming or if your function *intends* to mutate one of its arguments, then append the `&` character to the type of that parameter. This will pass the reference to the original object rather than passing a copy. In the following example the `pure_double` function looks like it mutates the argument before returning it. However it only doubles a *copy* of that argument and returns that, making it a pure function.

```c++
int pure_double(int a) {
  a *= 2;
  return a;
}
```

The `destructive_double` function will *update* the first argument to a number twice as large.

```c++
void destructive_double(int& a) {
  a *= 2;
}
```

## Arrays (introduction to...)

Arrays may be declared using this syntax.

```c++
int foo[n];
```

This example will implicitly create `n` integers using the default `int` constructor.   An array's size is set when it is created and is immutable. The array will always refer to the same collection of elements for its lifespan. 

Each `int` may be referenced by `foo[0],foo[1],...,foo[n-1]`. When an `int` is created with its default constructor, it will have whatever value happens to be in its location in memory. The values of an array may be set with `foo[3]=12`

To initialize all of the integers to a particular value at creation time, you can use any of these.

```c++
int foo[3] = {}; // 0,0,0
int foo[] = {0, 0, 0}; // 0,0,0
int foo[] = { 3, 0, 0, 0}; // 3,0,0,0
int foo[4] = { 3 }; // 3,0,0,0
int foo[] = {}; // empty
```

> Note
>
>Strangely, it is possible to reference indexes outside of an array's range, such as `foo[-1]` or `foo[n]`, without encountering an exception, but these values are not part of the declared array. Take care to avoid referencing these out of bounds indexes because there aren't guard rails to stop you!

## Data Structs

A Data Structure or `struct` is a compound data structure that is somewhat of a hybrid between a class and a value store. It can have several parameters of different types, and each property has its own name. For example, a rectangle could be represented as a length and a width.

```c++
struct rectangle {
  unsigned int width;
  unsigned int height;
};
```

You could use this struct to define an area function.

```c++
unsigned long area(rectangle rect) {
  return ((unsigned long) rect.width) * ((unsigned long) rect.height);
}

rectangle figure1;
figure1.height = 30;
figure1.width = 22;

cout << area(figure1) << endl; // 660
```

Data structures may also have constructors.

```c++
struct rectangle {
  unsigned int width;
  unsigned int height;
  rectangle(unsigned int w, unsigned int h) {
    width = w;
    height = h;
  }
}
```

You may also initialize each parameter by reference as part of a constructor.

```c++
struct rectangle {
  unsigned int width;
  unsigned int height;
  rectangle(unsigned int width, unsigned int height): width{width}, height{height} {}
};
```

The parameters of a data structure are public by default, so it is possible to reference them directly.

```c++
struct rectangle {
  unsigned int width;
  unsigned int height;
  rectangle(unsigned int width, unsigned int height): width{width}, height{height} {}
};

unsigned long rectangle_area(rectangle r) {
  return r.height * r.width;
}

```

A struct may also have an attribute that is a function.

```c++
struct rectangle {
  unsigned int width;
  unsigned int height;
  unsigned long area();
  rectangle(unsigned int width, unsigned int height): width{width}, height{height} {}
};

unsigned long rectangle::area() {
  return height * width;
}

```

A struct does not typically have member funtions, but usually they only hold collections of parameter values.

## Classes

### Access modifiers and "friend"

Classes are practically the same as structs but their members are private by default and they are typically used for data structures that are more than simple collections of parameters. Each parameter and method on a class may be `private`, `protected`, or `public`. The following example contains each of these access modifiers.

```c++
class Vehicle {
  unsigned int vin;
  int x;
  int y;
protected:
  bool move(int a, int b) {
    x += a;
    y += b;
  }
public:
  Vehicle(int x, int y, unsigned int vin): x{x}, y{y}, vin{vin} {}
  int distance() {
    return abs(y) + abs(x);
  }
};
```

Private attributes may be accessed only by instances of the class and by functions declared with the `friend` keyword.

### Operator overrides

The `friend` keyword is used in a lot of "operator override" methods. For example, it's possible to define the `==` relationship between two `Vehicle`s with the following.

```c++
class Vehicle {
  ...
public:
  ...
  friend bool operator==(const Vehicle& v1, const Vehicle& v2) {
    return v1.vin == v2.vin;
  }
};
```

Now the `==` operator will only check that the `vin` on two `Vehicle`s are the same.

```c++
#include <iostream>

int main() {
  Vehicle v1(37, 18, 123456), v2(-84, 22, 654321), v3(-84, 22, 123456);

  std::cout << "v1 == v2 is " << (v1 == v2) << std::endl;
  std::cout << "v1 == v3 is " << (v1 == v3) << std::endl;
  std::cout << "v2 == v3 is " << (v2 == v3) << std::endl;
  return 0;
}
```

This code will produce the following.

```sh
% g++ -std=c++20 ./main.cpp -o ./build/main
% ./build/main
v1 == v2 is 0
v1 == v3 is 1
v2 == v3 is 0
```

There are many other operators that may be overridden. The `BigInt` class [that is implemented as an exercise in this repo](./include/math/BigInt.hpp) provides several examples of these.

```c++
BigInt operator + (const BigInt&);
BigInt operator - (const BigInt&);
BigInt operator - ();
BigInt operator * (const BigInt&);
friend bool operator== (const BigInt&, const BigInt&);
friend ostream& operator<<(ostream&, const BigInt&);
```

Here is the complete list of overloadable operators in the c++ standard.

```
new delete
new[] ~!+-*/%^& | = += -= *= /= %= ^= &= |= == != < > <= >= <=> && || << >> <<= >>= ++ -- ,
```

The operators `+ - * &` may also be overloaded as either unary or binary operators.

Operators such as `==` may be implemented as class methods, but take care that you should be consistent that such an operator should be a const method.

### Virtual is the new abstract

### Class inheritance

## What are arrays really?

Let's consider this innocent looking class specification. It has a private character array, a constructor that accepts a character array as a parameter, and it has a public method that returns the length of this array.

```c++
class MyExampleClass {
  char content[];
public:
  MyExampleClass(char []);
  int content_length(void);
};
```

In programming languages like JavaScript, Python, or Java it would be trivial to implement this constructor and method. Let's see what happens when we implement the constructor in a completely reasonable *looking* way.

```c++
MyExampleClass::MyExampleClass(char in[]) {
  content = in;
}
```

This does not compile and the compiler shows this error.

```terminal
error: array type 'char[]' is not assignable
  content = in;
```

That's curious. The trouble is that every type in c takes up a fixed amount of memory. The memory that is allocated to an array depends on the number of elements in said array. In this instance the parameter `int content[];` is actually a pointer `int *content;` and it points to the location in memory where the first element of `char in[]` is stored. The type `char in[]` isn't really a type, since it can have an arbitrary size itself. You can only assign a fixed length character array, such as `char in[256]`, which allocates 256 consecutive locations in memory for values of type `char`.

We'll continue this discussion and get into how to manage pointers in classes further on.

### Standard libraries

#### iostream

The iostream library is used for logging with the `std::cout` command and the `<<` operator.

```c++
int a {-3}, b = 47;

std::cout << a << " > " << b << " is a " 
  << (a > b) << " statement.";
// -3 > 47 is a false statement.
```

Any class that overrides the `operator<<` method can be streamed with `<<`. The signature of this override looks like this.

```c++
class T {
public:
  friend ostream& operator<<(ostream&, const T&);
}
```

#### string

The `std::string` data type is part of the `<string>` library. 

This library has the useful `std::to_string` method that converts other types to strings and strings may be added with the `+` operator. The string type has useful helper methods `std::string::c_str`, which converts the string to the type used in exception messages, and `std::string::length`, which returns the number of characters in the string.

Most importantly the `std::string` type manages a variable amount of memory without the usual pitfalls that can occur when you do this yourself. If you don't clean up char arrays that are associated with your classes, then you will have memory leaks. When you copy a class instance, both instances of your class will have pointers to the same resource and if both instances attempt to de-allocate the referenced char arrays, then one of them will trhow an exception because the referenced array is already gone.

#### vector

The `std::vector<T>` data type is part of the `<vector>` library. It is a more robust way of managing a variable length collection of items. Like the `std::string` type it will avoid memory leaks and avoid issues with copying and deallocating memory.

This data type has a `std::vector<T>::push_back(T)` method that appends a new element to the end and a `std::vector<T>::pop_back()` method that removes the last element. It has a `std::vector<T>::size()` method that returns the size of the vector. The elements of a `std::vector<T> v` can be accessed by index via `v[i]`;

#### exception

The `<exception>` class has a virtual method `const char *std::exception::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW;` that returns the exception message. You may provide concrete exception classes that extend this and use them in `try { } catch( ) { }` blocks. You can use any type in your catch blocks, but it is best to use classes that extend `exception`. If the classes `FooException` and `BarException` both extend `exception`, then the following block will log a `FooException` message in the first block before handling it gracefully and when a `BarException` is thrown the process will terminate and log the result of calling `BarException::what()`.

```c++
try {
  //... do stuff
} catch(FooException fooEx) {
  std::cout << fooEx.what() << std::endl;
  handle_foo();
}
```

When a `catch` block contains an ellipsis, it will catch anything.

```c++
try {
  // ... do stuff
} catch (...) {
  std::cout << "Something was thrown." << std::endl;
}
```

#### chrono

This is the datetime library. It has several clock interfaces and the time_point and duration classes to manage timing data. The `time_clock` is intended for accurately capturing the current time if you simply want to know when something happened. The `steady_clock` is recommended if you're capturing a time duration but don't care as much about the absolute time.

Each clock has a `time_point now()` method that returns the instant when the method was invoked. When two `time_points` are subtracted, the result is a `duration`, which has a parameter that specifies the units for that duration. There is a constructor of the form `duration<T, U>duration(duration)` where `T` is the data type that it uses to report the time and `U` is a supported unit of time: hours, minutes, seconds, milli, micro, or nano. For example this is how to capture a duration in nanoseconds.

```c++

using namespace std::chrono;
...
steady_clock::time_point a = steady_clock::now();
// do stuff
steady_clock::time_point b = steady_clock::now();
duration<long, std::nano> d{b - a};
// d.count() returns the nanoseconds between a and b.
```

## Functions (using complex data structures and modifiers)

### Pointers and arrays in functions

A function's arguments must have consistent memory footprints so it is not possible to pass things like arrays with variable length as arguments to a function. 

### Pointers to functions

Suppose you want to implement a function decorator. Such a decorator needs to accept a pointer to another function as one of its arguments. A function pointer is declared like a normal function, but with `(*function_name)` instead of `function_name`.

The following example shows an implementation of a decorator that writes execution time to an output stream just before returning the result of the function execution.

```c++
#include <iostream>
#include <chrono>

unsigned long exec_with_time(unsigned long (*base)(unsigned short), std::ostream& out, unsigned short arg) {
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  unsigned long result = (*base)(arg);
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  out << chrono::duration<long, std::nano>(end - start).count() << "ns";
  return result;
}
```

## Managing pointers and memory in classes

### Pointers as class parameters

It's possible to use a c string as a class parameter if you try the following.

```c++
class MyExampleClass {
  const char *content;
public:
  MyExampleClass(const char *);
  int content_length(void);
  void print(void);
};
```

Let's implement this constructor and method in the following way.

```c++
MyExampleClass::MyExampleClass(const char *in) {
  content = in;
}
int MyExampleClass::content_length() {
  int length = 0;
  while (content[length++] != '\0') {
    length++;
  }
  return length;
}
void MyExampleClass::print() {
  std::cout << content << std::endl;
}

int main() {
  MyExampleClass instance{"Text content"};
  std::cout << "This example instance is " << instance.content_length() << " characters long." << std::endl;
  instance.print();
}
```

This compiles and has the expected console output.

> This example instance is 13 characters long.\
> Text content

The above works because strings such as "Text content" have the type `const char *`. What if we want to enhance the above class to have a `void append(const char *);` method? Since `content` is a const, we can't update it. How could we manage that? Let's try removing the `const` so that we have this declaration.

```c++
class MyExampleClass {
  char *content;
public:
  MyExampleClass(const char *);
  int content_length(void);
  void print(void);
  void append(const char *);
};
```

Now the constructor won't compile because the types of `content` and `in` don't match. Let's try initializing the contents as a character array and copying the constructor argument into that. Also let's extract the helper function to find the length of a string so that it can be re-used

```c++
int length_of(const char *);
int length_of(char *);
```

Here is a seemingly logical constructor for this class.

```c++
MyExampleClass::MyExampleClass(const char *in) {
  int len = length_of(in), i;
  char temp[len + 1];
  
  for (i = 0; i <= len; i++) {
  temp[i] = in[i];
  }
  content = temp;
}
```

It will compile, but then the following `main` method will print nonsense.

```c++
int main() {
  MyExampleClass instance{"Text content"};
  std::cout << "This example instance is " << instance.content_length() << " characters long." << std::endl;
  instance.print();
  return 0;
}
```

> This example instance is 0 characters long.\
`'?�&

The problem now is that `char temp[len + 1];` only preserves its block of memory while the constructor method is executing and then that memory is freed. This makes `content` a pointer to a block of memory that could contain anything. In order to persist this block of memory we need to be more heavy-handed. This is where we need to tread carefully. Once we persist some block of memory explicitly we need to take care to free that memory later to avoid memory leaks.

Here is a constructor that reserves the relevant block of memory.

```c++
MyExampleClass::MyExampleClass(const char *in) {
  int len = length_of(in), i;
  content = new char[len + 1];
  
  for (i = 0; i <= len; i++) {
  content[i] = in[i];
  }
}
```

Now running the `main` method has the expected output.

> This example instance is 12 characters long.\
Text content

How do we address the memory that was allocated to `content` now? There is a corresponding `delete[] content;` commant, which deallocates this memory. We can also declare a destructor method to call this deallocate method. That seems simple enough, but it leads to its own trouble. Let's add that destructor method and then see what happens when we use this class as a function argument. All together here is an example main.cpp file.

```c++
#include <iostream>

using namespace std;

int length_of(const char *);
int length_of(char *);

class MyExampleClass {
  char *content;
public:
  MyExampleClass(const char *);
  ~MyExampleClass();
  int content_length(void);
  void print(void);
};
void print_class_info(MyExampleClass);

int main() {
  MyExampleClass instance{"Text content"};
  print_class_info(instance);
  return 0;
}

MyExampleClass::MyExampleClass(const char *in) {
  int len = length_of(in), i;
  content = new char[len + 1];
  
  for (i = 0; i <= len; i++) {
  content[i] = in[i];
  }
}
// Destructor example
MyExampleClass::~MyExampleClass() {
  delete[] content;
}
int MyExampleClass::content_length() {
  return length_of(content);
}
void MyExampleClass::print() {
  std::cout << content << std::endl;
}

void print_class_info(MyExampleClass instance) {
  std::cout << "This example instance is " << instance.content_length() << " characters long." << std::endl;
  instance.print();
}

int length_of(const char *string) {
  int result = 0;
  while (string[result] != '\0') {
  result++;
  }
  return result;
}

int length_of(char *string) {
  int result = 0;
  while (string[result] != '\0') {
  result++;
  }
  return result;
}
```

This code will compile and run. It will even print the expected output. However it trhows an exception after the call to `print_class_info`.

> This example instance is 12 characters long.\
Text content\
free(): double free detected in tcache 2

That's a problem that arrises because the destructor is called when the underlying `content` is already deallocated. To test this, let's see what happens when we try to print the contents again after the method call resolves.

```c++
int main() {
  MyExampleClass instance{"Text content"};
  print_class_info(instance);
  instance.print();
  return 0;
}
```

> This example instance is 12 characters long.\
Text content\
�<�d\
free(): double free detected in tcache 2

How could we address this? The issue arrises because `void print_class_info(MyExampleClass);` references the example class *by value* and implicitly calls the `MyExampleClass::MyExampleClass(MyExampleClass&)` constructor, which has a default implementation that copies the value of `content` into a new class instance. We will need to instead create a new `content` for the new copy of the class in this constructor. Add this constructor implementation and restore the `main` method to its former content.

```c++
class MyExampleClass {
  ...
public:
  ...
  MyExampleClass(MyExampleClass&);
  ...
};
...
MyExampleClass::MyExampleClass(MyExampleClass& other) {
  int len = other.content_length(), i;
  content = new char[len + 1];
  for (i = 0; i <= len; i++) {
    content[i] = other.content[i];
  }
}
```

Finally this allows the example class to have an append method to update the contents.

```c++
class MyExampleClass {
  char *content;
public:
  MyExampleClass(const char *);
  MyExampleClass(MyExampleClass&);
  ~MyExampleClass();
  int content_length(void);
  void print(void);
  void append(const char *);
};
...
int main() {
  MyExampleClass instance{"Text content"};
  print_class_info(instance);
  instance.append(" with the appended text.");
  print_class_info(instance);
  return 0;
}
...
void MyExampleClass::append(const char *in) {
  int length = content_length(), new_length, cursor;
  new_length = length + length_of(in);
  char *new_content = new char[new_length + 1];
  for (cursor = 0; cursor <= new_length; cursor++) {
    if (cursor < length) {
      new_content[cursor] = content[cursor];
    } else {
      new_content[cursor] = in[cursor - length];
    }
  }
  delete[] content;
  content = new_content;
}
```

This now prints the following.

> This example instance is 12 characters long.\
Text content\
This example instance is 36 characters long.\
Text content with the appended text.

The example class demonstrates this specific issue with memory management, but there are other implicit ways to initialize a class such as `MyExampleClass instance = other_instance;`. If you manually manage memory, you will need to take extreme care to make sure that all of these operations properly manage allocating and deallocating memory. It is generally best to use the `std::string` class to manage variable length strings as parameters in classes.

## Preprocessors

### What's the deal with include and define?

### Macros

#### How to cheat with conSTRUCTors

This is a tale of my journey to writing my own testing library. This story begins innocently enough. It bothered me that my tests would take so long to compile and debug. I noticed that my compile times would only get out of hand when I used the boost framework to run my tests. I wasn't using anything fancy from the boost testing library, but the library as a whole is huge. What's more, every build ran into the same compiler warnings regarding the deprecated logging library they use. I wasn't a fan of the way the tests report exceptions either. Generally the boost testing framework has some quirks that I'm not such a big fan of. I eventually said to myself, I'll *just* replicate the functionality that I want and start building out a library that works the way my opinionated ass wants it to work.

Here is the challenge. How do you create a macro called `TEST` so that you can declare a test function and it is automatically registered with the test runner and executed as part of the test suite? This is obviously possible because it is done inside of the boost library, but how do they do it?

I started looking at the source code, and then at the c++ documentation, and back at the source code, and then tried to do some things myself... This is where I realized that something was weird. Where is the code that actually registers the tests? The Boost framework implements a lot of clever code to ensure that each test gets a unique name in the compiled code and creates each test case as an instance of a struct, but none of the subsequent code has a reference to that struct instance! I read each page of the spec for Macros hoping to find any way to pass the test name from one macro to another, but that functionality doesn't exist so far as I could find.

In desperation, I tried writing code that would just call the static register method on my test runner in the place where I create the test. No matter what I attempted I would run into one of two issues. Here is a sample program called `./example.cpp` that compiles and runs successfully.

```c++
#include <iostream>

using namespace std;

namespace wtf {
  class Runner {
    void (*runnable)();
  public:
    Runner(): runnable{nullptr} {};
    void run() {
      if (runnable != nullptr) {
        cout << "About to run test." << endl;
        (*runnable)();
        cout << "Completed running test." << endl;
      }
    }
    void reg(void (*r)()) {
      runnable = r;
    }
  } runner;

  void my_test();
  void my_test() {
    cout << "My test is running." << endl;
  }
}

int main() {
  wtf::runner.reg(&wtf::my_test);
  wtf::runner.run();
  return 0;
}
```

When you compile an run the code, you see these logs.

```sh
% g++ -std=c++20 ./explore.cpp -o ./build/explore
% ./build/explore
About to run test.
My test is running.
Completed running test.
```

Even though this works, it isn't remotely satisfactory. If I followed this patter, I'd have to go into `main` and add a call to every new test as I added them. Any test framework should allow you to run tests such as `my_test` from inside of `main` without adding a call to each test explicitly in `main`. I would like to use a macro so that a new test can be added in a test file and then it runs along with any other tests without updating `main.cpp`.

If you're generating that function name in a macro, it will be inaccessible from that context. I tried simply calling `runner.reg(&my_test)` where it is declared, but that doesn't compile.

```c++
#include <iostream>

using namespace std;

namespace wtf {
  ...
  void my_test();
  runner.reg(&my_test);
  void my_test() {
    cout << "My test is running." << endl;
  }
}

int main() {
  wtf::runner.run();
  return 0;
}
```

When `./explore.cpp` contains this code, the compiler shows an error.

```sh
% /usr/bin/g++ -std=c++20 ./explore.cpp -o ./build/explore
./explore.cpp:23:5: error: unknown type name 'runner'; did you mean 'Runner'?
  runner.reg(&my_test);
  ^~~~~~
  Runner
./explore.cpp:6:11: note: 'Runner' declared here
  class Runner {
      ^
./explore.cpp:23:11: error: cannot use dot operator on a type
  runner.reg(&my_test);
      ^
2 errors generated.
```

You can't execute functions outside of an execution context! Ok, what if I give this an execution context?!

```c++
#include <iostream>

using namespace std;

namespace wtf {
  ...
  void register_all() {
    void my_test();
    runner.reg(&my_test);
    void my_test() {
      cout << "My test is running." << endl;
    }
  }
}

int main() {
  wtf::register_all();
  wtf::runner.run();
  return 0;
}
```

Now we see the problem the other way.

```sh
% /usr/bin/clang++ -std=c++20 ./explore.cpp -o ./build/explore
./explore.cpp:25:24: error: function definition is not allowed here
    void my_test() {
             ^
1 error generated.
```

You can't create a function inside of an execution context! What gives?! How do you get off the ground with this?! This is where a sane person would live with the framework that others use even if it is obnoxious. I could keep using Boost tests, they work after all. I could instead avoid Boost by doing the dead simple, but ugly, thing and explicitly add each test call to `main.cpp`. 

I couldn't shake the thougt that "It's not necessary to do it this way!" I did not accept defeat and now I know how to get around the execution context issue! After digging through the boost code I finally figured out what code is generated from this prototypical test case.

```
BOOST_AUTO_TEST_CASE(as_decimal_string_simple)
{
  BigInt test_int((unsigned int []) { 4789 }, 1, false);
  BOOST_TEST(test_int.as_decimal_string() == "4789");
}
```

The auto test macro has this declaration.

```
#define BOOST_AUTO_TEST_CASE( test_name )               \
  BOOST_AUTO_TEST_CASE_NO_DECOR( test_name )          \
/**/
```

And the next macro...

```
#define BOOST_AUTO_TEST_CASE_NO_DECOR( test_name )     \
  BOOST_FIXTURE_TEST_CASE_NO_DECOR( test_name,       \
    BOOST_AUTO_TEST_CASE_FIXTURE )                 \
/**/
```

And the next (ignoring `BOOST_AUTO_TEST_CASE_FIXTURE` since it is a filler at the end of the line)...

```
#define BOOST_FIXTURE_TEST_CASE_NO_DECOR( test_name, F )   \
BOOST_FIXTURE_TEST_CASE_WITH_DECOR( test_name, F,          \
  boost::unit_test::decorator::collector_t::instance() ) \
/**/
```

And now the real one...

```
#define BOOST_FIXTURE_TEST_CASE_WITH_DECOR( test_name, F, decorators )  \
struct test_name : public F { void test_method(); };                    \
                                    \
static void BOOST_AUTO_TC_INVOKER( test_name )()                        \
{                                                                       \
  BOOST_TEST_CHECKPOINT('"' << #test_name << "\" fixture ctor");      \
  test_name t;                                                        \
  BOOST_TEST_CHECKPOINT('"' << #test_name << "\" fixture setup");     \
  boost::unit_test::setup_conditional(t);                             \
  BOOST_TEST_CHECKPOINT('"' << #test_name << "\" test entry");        \
  t.test_method();                                                    \
  BOOST_TEST_CHECKPOINT('"' << #test_name << "\" fixture teardown");  \
  boost::unit_test::teardown_conditional(t);                          \
  BOOST_TEST_CHECKPOINT('"' << #test_name << "\" fixture dtor");      \
}                                                                       \
                                    \
struct BOOST_AUTO_TC_UNIQUE_ID( test_name ) {};                         \
                                    \
BOOST_AUTO_TU_REGISTRAR( test_name )(                                   \
  boost::unit_test::make_test_case(                                   \
    &BOOST_AUTO_TC_INVOKER( test_name ),                            \
    #test_name, __FILE__, __LINE__ ),                               \
    decorators );                                                   \
                                    \
void test_name::test_method()                                           \
/**/
```

There's a lot to this last one, but we can now start to see what code is generated at the end of the line. A couple of these macros just append text to the supplied test name.

```
#define BOOST_AUTO_TC_INVOKER( test_name )      BOOST_JOIN( test_name, _invoker )
// as_decimal_string_simple -> as_decimal_string_simple_invoker
#define BOOST_AUTO_TC_UNIQUE_ID( test_name )    BOOST_JOIN( test_name, _id )
// as_decimal_string_simple -> as_decimal_string_simple_id
```

And this one is used several times.

```
#define BOOST_TEST_CHECKPOINT( M )                              \
  ::boost::unit_test::unit_test_log.set_checkpoint(           \
    BOOST_TEST_L(__FILE__),                                 \
    static_cast<std::size_t>(__LINE__),                     \
    (::boost::wrap_stringstream().ref() << M).str() )
#define BOOST_TEST_L( s )         ::boost::unit_test::const_string( s, sizeof( s ) - 1 )
// '"' << #test_name << "\" fixture ctor" -> 
//          ::boost::unit_test::unit_test_log.set_checkpoint("testFile.cpp", 55, "\"as_decimal_string_simple\" fixture ctor" )
// '"' << #test_name << "\" fixture setup" -> 
//          ::boost::unit_test::unit_test_log.set_checkpoint("testFile.cpp", 57, "\"as_decimal_string_simple\" fixture setup" )
// '"' << #test_name << "\" fixture entry" -> 
//          ::boost::unit_test::unit_test_log.set_checkpoint("testFile.cpp", 59, "\"as_decimal_string_simple\" fixture entry" )
// '"' << #test_name << "\" fixture teardown" -> 
//          ::boost::unit_test::unit_test_log.set_checkpoint("testFile.cpp", 61, "\"as_decimal_string_simple\" fixture teardown" )
// '"' << #test_name << "\" fixture dtor" -> 
//          ::boost::unit_test::unit_test_log.set_checkpoint("testFile.cpp", 61, "\"as_decimal_string_simple\" fixture dtor" )
```

Let's fill in what we have so far.

```
struct as_decimal_string_simple : public BOOST_AUTO_TEST_CASE_FIXTURE { void test_method(); };

static void as_decimal_string_simple_invoker()
{
  ::boost::unit_test::unit_test_log.set_checkpoint("testFile.cpp", 53, "\"as_decimal_string_simple\" fixture ctor" );
  as_decimal_string_simple t;
  ::boost::unit_test::unit_test_log.set_checkpoint("testFile.cpp", 53, "\"as_decimal_string_simple\" fixture setup" );
  boost::unit_test::setup_conditional(t);
  ::boost::unit_test::unit_test_log.set_checkpoint("testFile.cpp", 53, "\"as_decimal_string_simple\" fixture entry" );
  t.test_method();
  ::boost::unit_test::unit_test_log.set_checkpoint("testFile.cpp", 53, "\"as_decimal_string_simple\" fixture teardown");
  boost::unit_test::teardown_conditional(t);
  ::boost::unit_test::unit_test_log.set_checkpoint("testFile.cpp", 53, "\"as_decimal_string_simple\" fixture dtor");
}
                           
struct as_decimal_string_simple_id {};

BOOST_AUTO_TU_REGISTRAR( test_name )(
  boost::unit_test::make_test_case(
    &as_decimal_string_simple_invoker,
    "as_decimal_string_simple", "testFile.cpp", 53 ),
    boost::unit_test::decorator::collector_t::instance() );

void as_decimal_string_simple::test_method()
{
  BigInt test_int((unsigned int []) { 4789 }, 1, false);
  BOOST_TEST(test_int.as_decimal_string() == "4789");
}
```

Well that's something! We declare a struct with the name we provide, it extends the base test fixture and has a `test_method`. We then declare a method that creates an instance of this struct and runs the body with some logging enhancements. We then declare an empty struct type that ends with `_id` (whatever that does for us). Finally the `test_method` is defined to have the supplied body.

But what is this magical `BOOST_AUTO_TU_REGISTRAR( test_name )` and how does it *do* the registration?

```
#define BOOST_AUTO_TU_REGISTRAR( test_name )                       \
static boost::unit_test::ut_detail::auto_test_unit_registrar       \
BOOST_TEST_APPEND_UNIQUE_ID( BOOST_JOIN( test_name, _registrar ) ) BOOST_ATTRIBUTE_UNUSED \
/**/
// as_decimal_string_simple ->
//      boost::unit_test::ut_detail::auto_test_unit_registrar as_decimal_string_simple_registrar53
```

Well...ok? It declares an instance of another struct that has yet another unique name? What does *that* struct look like?

It turns out that...is another empty struct, but it has a number of overloaded constructors that don't *construct* anything. Here's the version of the constructor that the generated code calls.


```
auto_test_unit_registrar::auto_test_unit_registrar( boost::shared_ptr<test_unit_generator> tc_gen, decorator::collector_t& decorators )
{
  framework::current_auto_test_suite().add( tc_gen, decorators );
}
```

Son. Of. A... This is *exactly* what my `runner.reg(...)` method does, but it is executable outside of an execution context **because it's a constructor**! That's so simple and strange! You can shoehorn any side effects you want if you just put them in a constructor!

Now let's see how my implementation could work.

```c++
#include <iostream>
#include "Library.hpp"

using namespace std;

namespace wtf {
  ...
  struct cheater_struct {
    cheater_struct(void (*runnable)()) {
      runner.reg(runnable);
    }
  };

  void my_test();
  cheater_struct my_test_registrar(&my_test);
  void my_test() {
    cout << "My test is running." << endl;
  }
}

int main() {
  wtf::runner.run();
  return 0;
}
```

Bam! It was that simple the whole time. Now let's just define `./include/Library.hpp` that uses this idea...

```c++
#ifndef TEST_FRAMEWORK
#define TEST_FRAMEWORK

#include <iostream>
#include <exception>
#include <string>
#include <vector>

using namespace std;

namespace test {
  class AssertionFailure: public exception {
    string message;
  public:
    AssertionFailure(string message);
    virtual const char * what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
  };
  
  class Runner {
    void (*runnable)();
  public:
    Runner(void (*runnable)());
    Runner();
    void run();
  };
  class TestModule {
    static vector<Runner> runners;
  public:
    static void run_all();
    static void reg(Runner);
  };

  struct cheater_struct {
    cheater_struct(void (*runnable)());
  };
}

#define TEST(name) \
void name(); \
cheater_struct name ## _registrar(&name); \
void name() 
#endif
```

... with this implementation in `./Library.cpp`.

```c++
#include <Library.hpp>

#include <iostream>
#include <exception>
#include <string>

using namespace std;

namespace test {
  AssertionFailure::AssertionFailure(string message): message{message} {};
  const char * AssertionFailure::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return message.c_str();
  }
  
  Runner::Runner(void (*runnable)()): runnable{runnable} {};
  Runner::Runner(): Runner::Runner{nullptr} {}
  void Runner::run() {
    if (runnable != nullptr) {
      cout << "About to run test." << endl;
      try {
        (*runnable)();
      cout << "Completed running test." << endl;
      } catch(AssertionFailure e) {
        cerr << "Test failure: " << e.what() << endl;
      }
    }
  }

  void TestModule::run_all() {
    for (int i = 0; i < runners.size(); i++) {
      runners[i].run();
    }
  }
  void TestModule::reg(Runner runnable) {
    runners.push_back(runnable);
  }

  cheater_struct::cheater_struct(void (*runnable)()) {
    Runner next_runner(runnable);
    TestModule::reg(next_runner);
  }
}
```

And voila! You can create a test file `./testFile.cpp`.

```c++
#include <Library.hpp>

using namespace test;
using namespace std;

TEST(this_fails) {
  throw AssertionFailure("This didn't do the thing!");
}

TEST(this_passes) {
  cout << "This test ran!" << endl;
}

```

And then create a `./main.cpp`.

```c++
#include <Library.hpp>

using namespace test;

int main() {
  TestModule::run_all();
}

```

This will compile and run!

```sh
% /usr/bin/clang++ -std=c++20 -I./include ./*.cpp -o ./build/tests
%./build/tests
About to run test.
Test failure: This didn't do the thing!
About to run test.
This test ran!
Completed running test.
```
