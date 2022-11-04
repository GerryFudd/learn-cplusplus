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

## Data structures

### Native

#### Numbers

#### Text and junk

### Standard libraries

#### iostream

#### string

#### exception

#### algorithm

## Functions

## Pointers

### Arrays

## Classes

### Virtual is the new abstract

### Class inheritance


