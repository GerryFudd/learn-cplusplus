[Language properties](../language_properties.md)

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
