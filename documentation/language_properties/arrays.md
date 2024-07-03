[Language properties](../language_properties.md)

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
