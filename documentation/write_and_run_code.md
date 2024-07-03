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