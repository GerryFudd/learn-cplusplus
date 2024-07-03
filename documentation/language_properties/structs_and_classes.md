[Language properties](../language_properties.md)

## Structs

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

