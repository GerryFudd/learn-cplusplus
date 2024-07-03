[Language properties](../language_properties.md)

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
