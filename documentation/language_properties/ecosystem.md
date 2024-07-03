[Language properties](../language_properties.md)

## Standard libraries

### iostream

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

### string

The `std::string` data type is part of the `<string>` library. 

This library has the useful `std::to_string` method that converts other types to strings and strings may be added with the `+` operator. The string type has useful helper methods `std::string::c_str`, which converts the string to the type used in exception messages, and `std::string::length`, which returns the number of characters in the string.

Most importantly the `std::string` type manages a variable amount of memory without the usual pitfalls that can occur when you do this yourself. If you don't clean up char arrays that are associated with your classes, then you will have memory leaks. When you copy a class instance, both instances of your class will have pointers to the same resource and if both instances attempt to de-allocate the referenced char arrays, then one of them will trhow an exception because the referenced array is already gone.

### vector

The `std::vector<T>` data type is part of the `<vector>` library. It is a more robust way of managing a variable length collection of items. Like the `std::string` type it will avoid memory leaks and avoid issues with copying and deallocating memory.

This data type has a `std::vector<T>::push_back(T)` method that appends a new element to the end and a `std::vector<T>::pop_back()` method that removes the last element. It has a `std::vector<T>::size()` method that returns the size of the vector. The elements of a `std::vector<T> v` can be accessed by index via `v[i]`;

### exception

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

### chrono

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
