[Language properties](../language_properties.md)

## Variable declaration

Every variable is assigned a connected block of memory at the time it is declared. For this reason you must declare the type of the variable as it is created. You must even declare the size of compound data types.

All data types in c++ have constructors. So even integers and booleans may be initialized with statements like `int a {-3};` or `bool b {false};`. It is also possible to set a variable equal to an instance of its type `int a = -3;` or `bool b = false;`. When a type has a default constructor and an instance of the class is declared without a constructor call, such as `int c;`, that default constructor is implicitly called.

It is also possible to declare several variables of the same type separated by commas.

```c++
int a {-3}, b = 14, c(-8), d;
```

In this example `a, b, c,` and `d` are `int` type objects, `a == -3`, `b == 14`, `c == -8`, and `d` has an indeterminate value.
