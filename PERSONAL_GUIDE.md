# Personal guide to c++

Notes for my future self.

## How do you write and run code?

### How do you compile and run?

The command line tool for c++ is called `clang++` and on macos the command `c++` seems to be an alias. Source files in c++ end with `.cpp`. Here's how you write, compile, and execute such a file.

The entry point of a c++ project is a method called `main`, which returns an int and accepts no arguments. The program terminates in a success state if `main` returns 0 and in an error state otherwise.

The `std::cout` command, which is in the `iostream` library, prints to the console so that is part of the prototypical "Hello, world!" program. The `std::endl` command from the same library returns the environment-specific newline character, so we will use that as well.

Create a file called `main.cpp` and open it in a text editor. Write the following in that file.

```c++
#include <iostream>

int main() {
    std::cout << "Hello, world!" << std::endl;
    return 0;
}

```

To compile such a file and output the result to an executable run `%c++ <source file name>.cpp -o <output file name>`. Then you can execute the code with `%./<output file name>`. The flag `-std=#` tells the compiler to use a specific version of c++ during compilation. The latest version of c++ is 20 (at time of writing, with version 23 in development) so this is the version of c++ we will specify.

Now run `%c++ -std=c++20 main.cpp  -o ./hello` and notice that there is a new file called `hello` in your working directory. Execute this file.

```zsh
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

```zsh
% c++ -std=c++20 -I./include src/hello.cpp main.cpp -o ./import_test
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

All data types in c++ have constructors. So even integers and booleans may be initialized with statements like `int a {-3};` or `bool b {false};`. It is also possible to set a variable equal to an instance of its type `int a = -3;` or `bool b = false;`. When a type has a default constructor that is implicitly called when an instance of that type is declared without an explicit value assigned or constructor call such as `int c;`.

It is also possible to declare several variables of the same type separated by commas.

```c++
int a {-3}, b = 14, c(-8), d;
```

In this example `a, b, c,` and `d` are `int` type objects, `a == -3`, `b == 14`, `c == -8`, and `d` has an indeterminate value.

## Data structures

### Native

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

The `string` data type is part of the `<string>` library. This library also has the useful `std::to_string` method that converts other types to strings. Strings may also be added with the `+` operator.

The string type also has useful helper methods `std::string::c_str`, which converts the string to the type used in exception messages, and `std::string::length`, which returns the number of characters in the string.

#### exception

The `<exception>` class has a virtual method `std::exception::what` that returns the exception message. You may provide concrete exception classes extend this and use them in `try { } catch( ) { }` blocks. You can use any type in your catch blocks, but it is best to use classes that extend `exception`. If the classes `FooException` and `BarException` both extend `exception`, then the following block will log a `FooException` message in the first block and will log a generic message when a `BarException` is caught in the second block.

```c++
try {
    //... do stuff
} catch(FooException fooEx) {
    std::cout << fooEx.what() << std::endl;
} catch(exception e) {
    std::cout << "An unexpected exception was encountered" << std::endl;
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

This is the datetime library. It has a  

## Pointers (introduction to...)

A pointer is a one byte number that points to an address in memory. Pointers are declared as a reference to a specific data type so the variable they point to must be the declared type. An `*` in front of a pointer returns the variable that is stored int the address the pointer points to. A pointer is declared as `${type} * ${name}`. An `&` in front of a variable returns the variable's address in memory. If the variable takes up multiple bytes, this returns the address of the first byte.

```c++
int a = -43;
int * b = &a;
int c = *b;
int * d = &c;
```

In this example `b != d` but `*b == *d`. Likewise `a == c` but `&a != &c`.

## Functions (introduction to...)

### Declaration syntax

A function is declared with a return type and a collection of arguments. This is a function that returns the first index of the given character in the given string or -1 if the character is missing.

```c++
int index_of(char a[], unsigned short length, char b) {
    for (int i = 0; i < length; i++) {
        if (a[i] == b) {
            return i;
        }
    }
    return -1;
}
```

### Overloading

It is also possible to overload functions with the same name. The following pair of methods will expose a method that behaves identically to the one above but adds another where you can specify the index where you should start searching for the given character.

```c++
int index_of(char a[], unsigned short length, char b, unsigned short from) {
    for (unsigned short i = from; i < length; i++) {
        if (a[i] == b) {
            return i;
        }
    }
    return -1;
}

int index_of(char a[], unsigned short length, char b) {
    return index_of(a, length, b, 0);
}
```

### Parameters by reference

When a parameter is specified by its type and no modifiers the function references this argument *by value*. When the function is invoked, a copy of this variable is created with the same type and value. Practically this means two things.

1. The function incurs the processing overhead of copying the variable and the copy is stored in a new location in memory.
1. It is not possible to modify the argument that is passed to the function since it doesn't have a direct reference to the original.

For primitive types such as `int` or `char` the cost of copying the variable is negligible and in most cases it is a feature rather than a bug to avoid mutating the arguments that are passed to your functions.

If the process of copying one of your arguments is unacceptably time consuming or if your function *intends* to mutate one of its arguments, then append the `&` character to the type of that parameter. This will pass the reference to the original object rather than passing a copy. In the following example the `silly_double` function will mutate copies of its argument, accomplishing nothing.

```c++
void silly_double(int a) {
    a *= 2;
}
```

The `destructive_double` function will update the first argument to a number twice as large.

```c++
void destructive_double(int& a) {
    a *= 2;
}
```

## Arrays (introduction to...)

## Classes

### Virtual is the new abstract

### Class inheritance

## What are arrays really?

Let's consider this innocent looking class specification. It has a private character array, a constructor that accepts a character array as a parameter, and it has a public method that returns the length of this array.

```c++
class MyExampleClass {
    char content[];
public:
    MyExampleClass(char in[]);
    int content_length();
};
```

In programming languages like JavaScript, Python, or Java it would be trivial to implement this constructor and method.

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