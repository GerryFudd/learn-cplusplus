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
